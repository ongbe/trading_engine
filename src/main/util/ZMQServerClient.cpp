#include "ZMQServerClient.h"

ZMQServerClientBase::ZMQServerClientBase(const int zmq_type) : m_zmq_type(zmq_type)
{
  //--------------------------------------------------
  m_send_queue.reset(new ExpandableCirBuffer(1,4096,81920,CentralMemMgr::Instance()));
  m_recv_queue.reset(new ExpandableCirBuffer(1,4096,81920,CentralMemMgr::Instance()));
  //--------------------------------------------------
  m_zmqcontext.reset(new zmq::context_t(1));
  m_zmqsocket.reset(new zmq::socket_t(*m_zmqcontext, zmq_type));

  if (zmq_type == ZMQ_SUB) m_zmqsocket->setsockopt(ZMQ_SUBSCRIBE, "", 0);

  // const int val = 0;
  // const int rc = zmq_setsockopt(m_zmqsocket.get(), ZMQ_LINGER, &val, sizeof(val));
  // //--------------------------------------------------
  // // if this fails just let it be
  // //--------------------------------------------------
  // assert (rc == 0);
}

ZMQServer::ZMQServer(const int zmq_type) : ZMQServerClientBase(zmq_type) {}
ZMQClient::ZMQClient(const int zmq_type) : ZMQServerClientBase(zmq_type) {}

ZMQServer::ZMQServer(const int zmq_type, const string & sPort  ) : ZMQServerClientBase(zmq_type) { Set(sPort  ); }
ZMQClient::ZMQClient(const int zmq_type, const string & sIPPort) : ZMQServerClientBase(zmq_type) { Set(sIPPort); }

void ZMQServer::Set(const string & sPort)
{
  const string sConn = "tcp://*:"+sPort;
  m_zmqsocket->bind(sConn.c_str());
  Run();
}

void ZMQClient::Set(const string & sIPPort)
{
  const string sConn = "tcp://"+sIPPort;
  m_zmqsocket->connect(sConn.c_str());
  Run();
}

void ZMQServerClientBase::Send(const string & s)
{
  const size_t sz = s.length();
  BYTE * pw = m_send_queue->GetWritingPtr();
  memcpy((char*)pw,(char*)s.c_str(),sz);
  ((char*)pw)[sz] = '\0';
  m_send_queue->PushBack(sz+1);
}

void ZMQServerClientBase::Run()
{
  p_thd.reset(new boost::thread(&ZMQServerClientBase::SendRecvLoop, this));
}


void ZMQServerClientBase::KeepWaitingForRead()
{
  zmq::pollitem_t m_pollitems [] = {
    { (void *)m_zmqsocket.get(), 0, ZMQ_POLLIN, 0 }
  };
  do
  {
    zmq::poll (&m_pollitems[0], 1, -1);
    boost::this_thread::sleep(boost::posix_time::microseconds(10*1000));
  } while (!(m_pollitems[0].revents & ZMQ_POLLIN));
}

void ZMQServerClientBase::KeepWaitingForWrite()
{
  zmq::pollitem_t m_pollitems [] = {
    { (void *)m_zmqsocket.get(), 0, ZMQ_POLLOUT, 0 }
  };
  do
  {
    zmq::poll (&m_pollitems[0], 1, -1);
    boost::this_thread::sleep(boost::posix_time::microseconds(10*1000));
  } while (!(m_pollitems[0].revents & ZMQ_POLLOUT));
}


void ZMQServerClientBase::SendRecvLoop()
{
  zmq::message_t zmqm;
  unsigned long ulTS;
  BYTE * pb;

  bool bBusyLoop = true;
  for (;;)
  {
    if (m_zmq_type != ZMQ_SUB)
    {
      //--------------------------------------------------
      // send first
      //--------------------------------------------------
      if (!m_send_queue->EmptyNoLock() && m_send_queue->GetReadingPtrTStamp(pb,&ulTS))
      {
        const size_t sz = m_send_queue->GetPktSize();
        zmq::message_t zmq_send_msg(sz);
        memcpy((void *)zmq_send_msg.data(),pb,sz);
        // KeepWaitingForWrite();
        m_zmqsocket->send(zmq_send_msg);
        // m_zmqsocket->send(zmq_send_msg, ZMQ_NOBLOCK);
        m_send_queue->PopFront();
        bBusyLoop = false;
      }
    }

    if (m_zmq_type != ZMQ_PUB)
    {
      //--------------------------------------------------
      // recv
      //--------------------------------------------------
      // KeepWaitingForRead();
      const int rc = m_zmqsocket->recv(&zmqm, ZMQ_DONTWAIT);
      const size_t sz = zmqm.size();
      if (rc > 0)
      {
        BYTE * pw = m_recv_queue->GetWritingPtr();
        memcpy(pw,(char*)zmqm.data(),sz);
        ((char*)pw)[sz] = '\0';
        m_recv_queue->PushBack(sz);
        bBusyLoop = false;
      }
    }

    //--------------------------------------------------
    // prevent busy loop
    //--------------------------------------------------
    if (bBusyLoop) boost::this_thread::sleep(boost::posix_time::milliseconds(1));
    else           bBusyLoop = true;
  }
}

Tuple2<bool,string> ZMQServerClientBase::GetStr()
{
  unsigned long ulTS;
  BYTE * pb;

  if (m_recv_queue->EmptyNoLock() || !m_recv_queue->GetReadingPtrTStamp(pb,&ulTS))
  {
    return Tuple2<bool,string>(false,"");
  }
  else
  {
    const string sRtn((char*)pb,m_recv_queue->GetPktSize());
    m_recv_queue->PopFront();
    return Tuple2<bool,string>(true,sRtn);
  }
}

const size_t ZMQServerClientBase::GetSendQueueSize()
{
  return m_send_queue->Size();
}

const size_t ZMQServerClientBase::GetRecvQueueSize()
{
  return m_recv_queue->Size();
}


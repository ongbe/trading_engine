/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _MKTDATA_FLOAT_H_
#define _MKTDATA_FLOAT_H_

#if defined(SBE_HAVE_CMATH)
/* cmath needed for std::numeric_limits<double>::quiet_NaN() */
#  include <cmath>
#  define SBE_FLOAT_NAN std::numeric_limits<float>::quiet_NaN()
#  define SBE_DOUBLE_NAN std::numeric_limits<double>::quiet_NaN()
#else
/* math.h needed for NAN */
#  include <math.h>
#  define SBE_FLOAT_NAN NAN
#  define SBE_DOUBLE_NAN NAN
#endif

#if __cplusplus >= 201103L
#  include <cstdint>
#  include <functional>
#  include <string>
#  include <cstring>
#endif

#include <sbe/sbe.h>


using namespace sbe;

namespace mktdata {

class FLOAT
{
private:
    char *m_buffer;
    std::uint64_t m_bufferLength;
    std::uint64_t m_offset;
    std::uint64_t m_actingVersion;

    inline void reset(char *buffer, const std::uint64_t offset, const std::uint64_t bufferLength, const std::uint64_t actingVersion)
    {
        if (SBE_BOUNDS_CHECK_EXPECT(((offset + 8) > bufferLength), false))
        {
            throw std::runtime_error("buffer too short for flyweight [E107]");
        }
        m_buffer = buffer;
        m_bufferLength = bufferLength;
        m_offset = offset;
        m_actingVersion = actingVersion;
    }

public:
    FLOAT(void) : m_buffer(nullptr), m_offset(0) {}

    FLOAT(char *buffer, const std::uint64_t bufferLength, const std::uint64_t actingVersion)
    {
        reset(buffer, 0, bufferLength, actingVersion);
    }

    FLOAT(const FLOAT& codec) :
        m_buffer(codec.m_buffer), m_offset(codec.m_offset), m_actingVersion(codec.m_actingVersion) {}

#if __cplusplus >= 201103L
    FLOAT(FLOAT&& codec) :
        m_buffer(codec.m_buffer), m_offset(codec.m_offset), m_actingVersion(codec.m_actingVersion) {}

    FLOAT& operator=(FLOAT&& codec)
    {
        m_buffer = codec.m_buffer;
        m_bufferLength = codec.m_bufferLength;
        m_offset = codec.m_offset;
        m_actingVersion = codec.m_actingVersion;
        return *this;
    }

#endif

    FLOAT& operator=(const FLOAT& codec)
    {
        m_buffer = codec.m_buffer;
        m_bufferLength = codec.m_bufferLength;
        m_offset = codec.m_offset;
        m_actingVersion = codec.m_actingVersion;
        return *this;
    }

    FLOAT &wrap(char *buffer, const std::uint64_t offset, const std::uint64_t actingVersion, const std::uint64_t bufferLength)
    {
        reset(buffer, offset, bufferLength, actingVersion);
        return *this;
    }

    static const std::uint64_t encodedLength(void)
    {
        return 8;
    }


    static const std::int64_t mantissaNullValue()
    {
        return SBE_NULLVALUE_INT64;
    }

    static const std::int64_t mantissaMinValue()
    {
        return -9223372036854775807L;
    }

    static const std::int64_t mantissaMaxValue()
    {
        return 9223372036854775807L;
    }

    std::int64_t mantissa(void) const
    {
        return SBE_LITTLE_ENDIAN_ENCODE_64(*((std::int64_t *)(m_buffer + m_offset + 0)));
    }

    FLOAT &mantissa(const std::int64_t value)
    {
        *((std::int64_t *)(m_buffer + m_offset + 0)) = SBE_LITTLE_ENDIAN_ENCODE_64(value);
        return *this;
    }

    static const std::int8_t exponentNullValue()
    {
        return SBE_NULLVALUE_INT8;
    }

    static const std::int8_t exponentMinValue()
    {
        return (std::int8_t)-127;
    }

    static const std::int8_t exponentMaxValue()
    {
        return (std::int8_t)127;
    }

    std::int8_t exponent(void) const
    {
        return (std::int8_t)-7;
    }
};
}
#endif

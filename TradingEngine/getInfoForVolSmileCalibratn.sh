cat /mnt/d/nir/nirvana.log | grep Moneyness | grep -P "=\t1.1000" | grep -v " 09:15:00" | grep -v " 09:16:00" | grep -v " 09:17:00" | grep -v " 09:18:00" | grep -v " 2013-02" | grep -v " 2013-03" | grep -v " 2015-01" | grep -v " 2015-02" | grep -v "\-nan" > /mnt/w/tmp.txt

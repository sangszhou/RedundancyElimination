Description
====
This project is used to analysis the redundancy rate in enterprising WLAN network, and it contains four major phases, parse user log, dispatch packets, pattern extraction and redundancy estimation

##Data set
We collected real trace data over two weeks from Jan 1 to Jan 14 in the year of 2013 which involes 10,000 users and the total data volume exceeds 18T.

##Phase one Parse User Log
The data set comes with an extra user log information table which record the detailed information about when a user connect (disconnect) to WLAN and which IP address is assigned during this connection. Those information are important because it can tell us a packet's owner. 

The raw user log information can be found [here](https://www.dropbox.com/s/wlg4rmj7nvuttgv/RADIUS%20Accounting%20table.csv). This is only a small snippet of original log information but should be enough to express the format.
And the formatted user log after parsing can be found [here](https://github.com/sangszhou/RedundancyElimination/blob/master/ParseCSVTest/IPInfo.txt).

The biggest challenge I faced during this phase is information repairing. That is, like many real data trace, the log information table is not flawless. For example, there exists rows indicate a user's connection to WLAN but no corresponding disconnection information or rows without assigned IP address field. When dispatching packets, I found that extra 25% of the total packets are remained because of information repairing.

The size of data set requires designing formatted log information in a way that providing fastest query speed. For example, it may be hard to debug when describing time in the format of linux time stamp(like 1357288790) than that of readable format(like, 20:16:59), but the first altenative is much faster when query a packet's owner.

##Phrase two Dispatch packets
In this phase, we classify packets according to their time stamp and IP address. For each packet, we extract corresponding time stamp and IP address and query it in user log table and an ID(user mac address) can be obtained, and then we group all packets into same file which have same ID.

Those packets follow [CAPWAP protocal](http://en.wikipedia.org/wiki/CAPWAP) and are readable in [wireshark](http://www.wireshark.org/). An alternative to me to read/write packets is Winpcap(Libpcap). I wrote code using Winpcap in Visual studio under windows operating system and it turned out that it is far too slow(60s/190M). So, efficiency issues become the biggest challenge in this phase.

The principles I followed to handle the efficiency issues are:
1. Switch to linux operating system to get faster IO speed and more efficient STL implementation. 
2. Redesign user log table data structure by switching time description from string to int.
3. Abandon Winpcap and implement my own one to avoid calling functions.
4. Refuse using local varible unless necessary.
5. Refuse calling method unless necessary.
6. Refuse using STL unless necessary.
By following above priciples, I increased the performance dramatically(10s/190M). Note that, there still other principles may improve the performance like write buffer. But 10s/190M already meet my demand so I didn't try that one.

##Phase three pattern extraction and redundancy elimination
We have already merged those packets which have same ID into one file. In this phase, we gonna extract the data pattern of each user and calculate user's redundancy rate.
I faced performace issues in this phrase too, but unlike last phrase, the bottleneck is not IO anymore but the limit of computing capacity.

There are also principles to deal with it:
1. Use [murmurhash](http://en.wikipedia.org/wiki/MurmurHash) to generate fingerprints. Theoretically, Rabin Karp algorithm should yield better performance, but RK needs big integer. At last, I found that due to the introduction of gmpxx.libray(used to present big integer), RK algorithm is slower than violent solution. 
2. Use bloom filter to help calculate optimal redundancy rate
3. Sampling data set 
4. Multithreading solution
the final performance is about 25s/110M, and I see no more improvement.

##Summary
1. For each phase, I developed a quick and dirty program first, trying to find where the bottleneck is and came up with principles to deal with it. I saved a lot of time by doing this.
2. I have been tought the importance of object-oriented programming, and OO principles are so deep in my vein that I despise any codes that smells like procedure-orinted. For the first time, I witness the beauty of PO programming. 


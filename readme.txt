Group : 

Ajay Gahlot 204101004
Sarthak Agarwal 204101049

For Question 1 ------>       

Filename  -------- 1.cpp


To compile  --------  g++ 1.cpp -o routing

To run ----------   

./routing -top top14.doc -conn NSFNET_100.doc -rt routingfile.txt -ft forwarding.txt -path pathfile.txt -flag dist -p 1

./routing -top top24.doc -conn ARPANET_100.doc -rt routingfile.txt -ft forwarding.txt -path pathfile.txt -flag hop -p 0


p value can be change according to optimmistic or pessimistic (0/1 respectively)

flag can be hop or dist

four output files are generated

1. routingfile.txt  ---------->       source | destination | path | cost of path  (there two entries for every pair since two shortest path)

2. forwarding.txt  ------------->    for every we have node created forwarding table interface_in | label_in | interface_out | label_out if -1 then indicates node is 					      source or destination node

3. pathfile.txt  --------------> for all the admitted connection connection_id | source | destition | label_list | cost

4. output.txt --------------> no of suceesful admitted connection




For Question 2 ------------>

Filename : 2.cpp
To compile : g++ 2.cpp
To run : ./a.out


Output is on console as well as in the output.txt file





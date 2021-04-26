#include<bits/stdc++.h>
using namespace std;

double total_simulation_time = 10000000; // total time for simulation is done(not required done for 100000 workers)

double probability = 0.2; // for case C

// worker arrival and departure
struct Worker
{
    double arrival_time_of_worker;
    double  departure_time_of_worker;
};
// to get exponential distribution
double get_exponential_value(double val) {
    double num1 = ((double) rand() + 1) / ((double) (RAND_MAX) + 1);
    double num2 = (double)(-(log(1 - num1)) / val);

    return num2;
}
// CaseA:
void caseA(double mean_arrival_rate,double mean_service_rate)
{

    double previous_time=0; // time when  previous event happen 

    double curr_arrival_time = 0; // time when next arrival will happen

    double curr_departure_time1 = 0,curr_departure_time2 = 0; // time when next departure will happen for each officer

    double total_waited_time1 = 0,total_waited_time2 = 0; // total waiting time for all the workers

    double all_total_system_time1 = 0,all_total_system_time2 = 0; // total time spent by all the workers who come inside the system 

    double process_total_system_time1 = 0,process_total_system_time2 = 0; // total time spent by all the workers who come have completed their service process 

    double curr_time1 = 0,curr_time2=0; // current time

    double total_workers1 = 0,total_workers2 = 0; // total no of workers for each officer

    double total_workers_waited1 = 0,total_workers_waited2 = 0; // total no workers which has to wait in queue
    
    double total_workers_processed1 = 0,total_workers_processed2 = 0; //total no workers who come have completed their service process 


    list<Worker>l1,l2; // to maintain the process inside the queue
    
    int num = 1; // to which queue assigned
    
    /* for queue 1*/

    while(total_workers1 < 10000)
    {
        
        previous_time = curr_time1;
        
        if(l1.empty() == true)
        {
            	curr_departure_time1 = total_simulation_time;
        }
        else
        {
        	struct Worker w = l1.front();
            	curr_departure_time1 = w.departure_time_of_worker;
        }	
        if(curr_departure_time1 > curr_arrival_time)
        {

            	curr_time1 = curr_arrival_time;
        }
        else
        {

            	curr_time1 = curr_departure_time1;
        }
        
        all_total_system_time1 += l1.size()*(curr_time1-previous_time);

        if(curr_arrival_time < curr_departure_time1)
        {
         		
         		total_workers1++;
            		struct Worker w;
            		w.arrival_time_of_worker = curr_time1;
            		l1.push_back(w);
			
            		if(l1.size() == 1)
            		{
                    		double num = get_exponential_value(mean_service_rate);
                    		l1.front().departure_time_of_worker = curr_time1 + num;
                    		curr_departure_time1 = curr_time1 + num;
            		}
            		double num = get_exponential_value(mean_arrival_rate);
            		
            		curr_arrival_time += num;
        }
        else
        {
        
            		if(l1.size() > 0)
            		{
                		struct Worker w = l1.front();

                		process_total_system_time1  += ( w.departure_time_of_worker - w.arrival_time_of_worker);
                
                		total_workers_processed1++;

                		l1.pop_front();

                		if(l1.size() > 0)
                		{
                    			struct Worker w = l1.front(); 

                    			 total_waited_time1 += curr_time1 - w.arrival_time_of_worker;
                    
                    			if(curr_time1 - w.arrival_time_of_worker > 0)
                    			{
                    				total_workers_waited1++;
                    			}

                    			double num = get_exponential_value(mean_service_rate);;

                    			curr_departure_time1 = curr_time1 + num;

                    			l1.front().departure_time_of_worker = curr_departure_time1;
                		}
            		}
        	
           }
    }

    curr_arrival_time = 0;
    previous_time = 0;
   
    
     while(total_workers2 < 10000)
    {
        
        previous_time = curr_time2;
        
        if(l2.empty() == true)
        {
            	curr_departure_time2 = total_simulation_time;
        }
        else
        {
        	struct Worker w = l2.front();
            	curr_departure_time2 = w.departure_time_of_worker;
        }	
        if(curr_departure_time2 > curr_arrival_time)
        {

            	curr_time2 = curr_arrival_time;
        }
        else
        {

            	curr_time2 = curr_departure_time2;
        }
        
        all_total_system_time2 += l2.size()*(curr_time2-previous_time);

        if(curr_arrival_time < curr_departure_time2)
        {
         		
         		total_workers2++;
            		struct Worker w;
            		w.arrival_time_of_worker = curr_time2;
            		l2.push_back(w);

            		if(l2.size() == 1)
            		{
                    		double num = get_exponential_value(mean_service_rate);
                    		l2.front().departure_time_of_worker = curr_time2 + num;
                    		curr_departure_time2 = curr_time2 + num;
            		}
            		double num = get_exponential_value(mean_arrival_rate);
            		//cout<<num<<"\n";
            		curr_arrival_time += num;
        }
        else
        {
        
            		if(l2.size() > 0)
            		{
                		struct Worker w = l2.front();

                		process_total_system_time2  += ( w.departure_time_of_worker - w.arrival_time_of_worker);
                
                		total_workers_processed2++;

                		l2.pop_front();

                		if(l2.size() > 0)
                		{
                    			struct Worker w = l2.front(); 

                    			 total_waited_time2 += curr_time2 - w.arrival_time_of_worker;
                    
                    			if(curr_time2 - w.arrival_time_of_worker > 0)
                    			{
                    				total_workers_waited2++;
                    			}

                    			double num = get_exponential_value(mean_service_rate);

                    			curr_departure_time2 = curr_time2 + num;

                    			l2.front().departure_time_of_worker = curr_departure_time2;
                		}
            		}
        	
           }
    }
    
    
    ofstream MyFile("output.txt");
    
  //  cout<<total_workers1<<"\n";
    cout<<"For queue 1:\n";
    
    cout<<"Average time for which a worker has to wait until getting checked: "<<(total_waited_time1)/(total_workers_processed1)<<"\n";
    
    cout<<"Average number of workers waiting in the queue before each officer: "<<total_workers_waited1/(total_workers1)<<"\n";
     
    cout<<"Average number of workers getting checked :"<<all_total_system_time1/curr_time1<<"\n";
    
    cout<<"Average response time for workers in getting checked :"<<(process_total_system_time1)/(total_workers_processed1 )<<"\n";
    
    
    cout<<"For queue 2:\n";
    
    cout<<"Average time for which a worker has to wait until getting checked: "<<(total_waited_time2)/(total_workers_processed2)<<"\n";
    
    cout<<"Average number of workers waiting in the queue before each officer: "<<total_workers_waited2/(total_workers2)<<"\n";
     
    cout<<"Average number of workers getting checked :"<<all_total_system_time2/curr_time2<<"\n";
    
    cout<<"Average response time for workers in getting checked :"<<(process_total_system_time2)/(total_workers_processed2 )<<"\n";
    
    
    MyFile<<"For queue 1:\n";
    
    MyFile<<"Average time for which a worker has to wait until getting checked: "<<(total_waited_time1)/(total_workers_processed1)<<"\n";
    
    MyFile<<"Average number of workers waiting in the queue before each officer: "<<total_workers_waited1/(total_workers1)<<"\n";
     
    MyFile<<"Average number of workers getting checked :"<<all_total_system_time1/curr_time1<<"\n";
    
    MyFile<<"Average response time for workers in getting checked :"<<(process_total_system_time1)/(total_workers_processed1 )<<"\n";
    
    
    MyFile<<"For queue 2:\n";
    
    MyFile<<"Average time for which a worker has to wait until getting checked: "<<(total_waited_time2)/(total_workers_processed2)<<"\n";
    
    MyFile<<"Average number of workers waiting in the queue before each officer: "<<total_workers_waited2/(total_workers2)<<"\n";
     
    MyFile<<"Average number of workers getting checked :"<<all_total_system_time2/curr_time2<<"\n";
    
    MyFile<<"Average response time for workers in getting checked :"<<(process_total_system_time2)/(total_workers_processed2 )<<"\n";
	
    MyFile.close();	
}
 

void caseB(double mean_arrival_rate,double mean_service_rate)
{

    double previous_time = 0;

    double curr_arrival_time = 0;

    double curr_departure_time1 = 0;
    
    double curr_departure_time2 = 0;

    double total_waited_time = 0;

    double all_total_system_time = 0;

    double processed_total_system_time = 0;

    double curr_time = 0;

    double total_workers = 0;

    double total_workers_waited = 0;
    
    double total_workers_processed = 0;
    
    int officer1 = 1,officer2 = 1;
    
    double index,temp;

    list<Worker>l;
    
    curr_departure_time1 = total_simulation_time;
    curr_departure_time2 = total_simulation_time;
    
  //  int count = 0;
   
    while(total_workers < 10000)
    {
	
	
        previous_time = curr_time;
        
        if(curr_departure_time1 < curr_departure_time2)
      	 {
      	 	temp = curr_departure_time1;
      	 	index = 1;
      	 }
      	 else
      	 {
      	 	temp = curr_departure_time2 ;
      	 	index = 2;
      	 }
      	 
        if(temp > curr_arrival_time)
        {

            curr_time = curr_arrival_time;
        }
        else
        {

            curr_time = temp;
        }
        
        all_total_system_time += l.size()*(curr_time-previous_time);
        
       // cout<<curr_arrival_time<<"\n";

        if(curr_arrival_time < temp)
        {
        	//cout<<"ok world\n";
         	total_workers++;
                struct Worker w;
                w.arrival_time_of_worker = curr_time;
        
                if(officer1 == 1)
                {
                    total_workers_processed++;
                 //   total_waited++;
                    double num = get_exponential_value(mean_service_rate);
                    all_total_system_time += num;
                    processed_total_system_time += num;
                    //l.front().departure_time_of_worker = curr_time + num;
                    curr_departure_time1 = curr_time + num;
                    officer1 = 0;
                }
                else if(officer2 == 1)
                {
                	total_workers_processed++;
                	//total_waited++;
                	double num = get_exponential_value(mean_service_rate);
                	all_total_system_time += num;
                    	processed_total_system_time += num;
                    //	l.front().departure_time_of_worker = curr_time + num;
                    	curr_departure_time2 = curr_time + num;
                    	officer2 = 0;
                }
                else
                {
                	l.push_back(w);
                }
		
            	double num = get_exponential_value(mean_arrival_rate);
            	curr_arrival_time += num;
        }
        else
        {
           	//cout<<"bye world\n";
           	
           	
           //	cout<<index<<"\n";
                
                if(index == 1)
                {
                	
                	curr_departure_time1 = total_simulation_time;
                	officer1 = 1;
                }
                
                else
                {
                	curr_departure_time2 = total_simulation_time;
                	officer2 = 1;
                }
                
		if(l.size() > 0)
                {
               // 	cout<<"ok world\n";
                    	struct Worker w = l.front(); 

		//	cout<<curr_time<<" "<<w.arrival_time_of_worker<<"\n";
                    	total_waited_time += curr_time - w.arrival_time_of_worker;
                    
                    	

                    	double num = get_exponential_value(mean_service_rate);
			
			if(index == 1)
			{
                       	curr_departure_time1 = curr_time + num;
                       	officer1 = 0;
                       	
                       }
                       else
                       {
                       	curr_departure_time2 = curr_time + num;
                       	officer2 = 0;
                       }
                       
                       if(curr_time - w.arrival_time_of_worker > 0)
                    	{
                    		total_workers_waited++;
                    	}
                       
			total_workers_processed++;
			all_total_system_time += num;
                    	l.pop_front();
                    	processed_total_system_time += ( curr_time + num - w.arrival_time_of_worker);
                
              }
               
        }
    }  
    
    ofstream MyFile("output.txt");

    cout<<"Average time for which a worker has to wait until getting checked: "<<(total_waited_time)/(total_workers_processed)<<"\n";
    
    cout<<"Average number of workers waiting in the queue before each officer: "<<total_workers_waited/(total_workers)<<"\n";
     
    cout<<"Average number of workers getting checked :"<<all_total_system_time/curr_time<<"\n";
    
    cout<<"Average response time for workers in getting checked :"<<(processed_total_system_time)/(total_workers_processed)<<"\n";
    
    
    MyFile<<"Average time for which a worker has to wait until getting checked: "<<(total_waited_time)/(total_workers_processed)<<"\n";
    
    MyFile<<"Average number of workers waiting in the queue before each officer: "<<total_workers_waited/(total_workers)<<"\n";
     
    MyFile<<"Average number of workers getting checked :"<<all_total_system_time/curr_time<<"\n";
    
    MyFile<<"Average response time for workers in getting checked :"<<(processed_total_system_time)/(total_workers_processed)<<"\n";
    
    MyFile.close();
}

void caseC(double mean_arrival_rate,double mean_service_rate)
{

  

    double previous_time=0; // time when  previous event happen 

    double curr_arrival_time = 0; // time when next arrival will happen

    double curr_departure_time1 = 0,curr_departure_time2 = 0; // time when next departure will happen for each officer

    double total_waited_time1 = 0,total_waited_time2 = 0; // total waiting time for all the workers

    double all_total_system_time1 = 0,all_total_system_time2 = 0; // total time spent by all the workers who come inside the system 

    double process_total_system_time1 = 0,process_total_system_time2 = 0; // total time spent by all the workers who come have completed their service process 

    double curr_time1 = 0,curr_time2=0; // current time

    double total_workers1 = 0,total_workers2 = 0; // total no of workers for each officer

    double total_workers_waited1 = 0,total_workers_waited2 = 0; // total no workers which has to wait in queue
    
    double total_workers_processed1 = 0,total_workers_processed2 = 0; //total no workers who come have completed their service process
    
    int workers_dropped1=0,workers_dropped2=0; 


    list<Worker>l1,l2; // to maintain the process inside the queue
    
    
     while(total_workers1 < 10000)
    {
        
        previous_time = curr_time1;
        
        if(l1.empty() == true)
        {
            	curr_departure_time1 = total_simulation_time;
        }
        else
        {
        	struct Worker w = l1.front();
            	curr_departure_time1 = w.departure_time_of_worker;
        }	
        if(curr_departure_time1 > curr_arrival_time)
        {

            	curr_time1 = curr_arrival_time;
        }
        else
        {

            	curr_time1 = curr_departure_time1;
        }
        
        all_total_system_time1 += l1.size()*(curr_time1-previous_time);
        
         if(curr_arrival_time < curr_departure_time1)
        {
         		
         		total_workers1++;
         		
         		
            		if(l1.size() == 5)
            		{
                		workers_dropped1++;
            		}
            		else
            		{
            			struct Worker w;
            			w.arrival_time_of_worker = curr_time1;
            			l1.push_back(w);

            			if(l1.size() == 1)
            			{
                    			double num = get_exponential_value(mean_service_rate);
                    			l1.front().departure_time_of_worker = curr_time1 + num;
                    			curr_departure_time1 = curr_time1 + num;
            			}
            			
            		}
            		double num = get_exponential_value(mean_arrival_rate);
            			//cout<<num<<"\n";
            		curr_arrival_time += num;
        }
        else
        {
        
            		if(l1.size() > 0)
            		{
                		struct Worker w = l1.front();

                		process_total_system_time1  += ( w.departure_time_of_worker - w.arrival_time_of_worker);
                
                		total_workers_processed1++;

                		l1.pop_front();

                		if(l1.size() > 0)
                		{
                    			struct Worker w = l1.front(); 

                    			 total_waited_time1 += curr_time1 - w.arrival_time_of_worker;
                    
                    			if(curr_time1 - w.arrival_time_of_worker > 0)
                    			{
                    				total_workers_waited1++;
                    			}

                    			double num = get_exponential_value(mean_service_rate);

                    			curr_departure_time1 = curr_time1 + num;

                    			l1.front().departure_time_of_worker = curr_departure_time1;
                		}
            		}
        	
           }
      }
    
   
    curr_arrival_time = 0;
    previous_time = 0;
    while(total_workers2 < 10000)
    {
        
        previous_time = curr_time2;
        
        if(l2.empty() == true)
        {
            	curr_departure_time2 = total_simulation_time;
        }
        else
        {
        	struct Worker w = l2.front();
            	curr_departure_time2 = w.departure_time_of_worker;
        }	
        if(curr_departure_time2 > curr_arrival_time)
        {

            	curr_time2 = curr_arrival_time;
        }
        else
        {

            	curr_time2 = curr_departure_time2;
        }
        
        all_total_system_time2 += l2.size()*(curr_time2-previous_time);
        
         if(curr_arrival_time < curr_departure_time2)
        {
         		
         		total_workers2++;
         		
         		
            		if(l2.size() == 5)
            		{
                		workers_dropped2++;
            		}
            		else
            		{
            			struct Worker w;
            			w.arrival_time_of_worker = curr_time2;
            			l2.push_back(w);

            			if(l2.size() == 1)
            			{
                    			double num = get_exponential_value(mean_service_rate);
                    			l2.front().departure_time_of_worker = curr_time2 + num;
                    			curr_departure_time2 = curr_time2 + num;
            			}
            			
            		}
            		double num = get_exponential_value(mean_arrival_rate);
            			//cout<<num<<"\n";
            			curr_arrival_time += num;
        }
        else
        {
        
            		if(l2.size() > 0)
            		{
                		struct Worker w = l2.front();

                		process_total_system_time2  += ( w.departure_time_of_worker - w.arrival_time_of_worker);
                
                		total_workers_processed2++;

                		l2.pop_front();

                		if(l2.size() > 0)
                		{
                    			struct Worker w = l2.front(); 

                    			 total_waited_time2 += curr_time2 - w.arrival_time_of_worker;
                    
                    			if(curr_time2 - w.arrival_time_of_worker > 0)
                    			{
                    				total_workers_waited2++;
                    			}

                    			double num = get_exponential_value(mean_service_rate);

                    			curr_departure_time2 = curr_time2 + num;

                    			l2.front().departure_time_of_worker = curr_departure_time2;
                		}
            		}
        	
           }
      }
      
    ofstream MyFile("output.txt");
      
    cout<<"For queue 1:\n";
    
    cout<<"Average time for which a worker has to wait until getting checked: "<<(total_waited_time1)/(total_workers_processed1)<<"\n";
    
    cout<<"Average number of workers waiting in the queue before each officer: "<<total_workers_waited1/(total_workers1)<<"\n";
     
    cout<<"Average number of workers getting checked :"<<all_total_system_time1/curr_time1<<"\n";
    
    cout<<"Average response time for workers in getting checked :"<<(process_total_system_time1)/(total_workers_processed1 )<<"\n";
    
    
    cout<<"For queue 2:\n";
    
    cout<<"Average time for which a worker has to wait until getting checked: "<<(total_waited_time2)/(total_workers_processed2)<<"\n";
    
    cout<<"Average number of workers waiting in the queue before each officer: "<<total_workers_waited2/(total_workers2)<<"\n";
     
    cout<<"Average number of workers getting checked :"<<all_total_system_time2/curr_time2<<"\n";
    
    cout<<"Average response time for workers in getting checked :"<<(process_total_system_time2)/(total_workers_processed2 )<<"\n";
    
    MyFile<<"For queue 1:\n";
    
    MyFile<<"Average time for which a worker has to wait until getting checked: "<<(total_waited_time1)/(total_workers_processed1)<<"\n";
    
    MyFile<<"Average number of workers waiting in the queue before each officer: "<<total_workers_waited1/(total_workers1)<<"\n";
     
    MyFile<<"Average number of workers getting checked :"<<all_total_system_time1/curr_time1<<"\n";
    
    MyFile<<"Average response time for workers in getting checked :"<<(process_total_system_time1)/(total_workers_processed1 )<<"\n";
    
    
    MyFile<<"For queue 2:\n";
    
    MyFile<<"Average time for which a worker has to wait until getting checked: "<<(total_waited_time2)/(total_workers_processed2)<<"\n";
    
    MyFile<<"Average number of workers waiting in the queue before each officer: "<<total_workers_waited2/(total_workers2)<<"\n";
     
    MyFile<<"Average number of workers getting checked :"<<all_total_system_time2/curr_time2<<"\n";
    
    MyFile<<"Average response time for workers in getting checked :"<<(process_total_system_time2)/(total_workers_processed2 )<<"\n";

    MyFile.close();

}

int main()
{
    
    
    while(true)
    {
    	double mean_arrival_rate , mean_service_rate;
    
    	cout<<"Enter the mean arrival rate\n";
    
    	cin>>mean_arrival_rate; 
    
     	cout<<"Enter the mean service rate\n";
    
    	cin>>mean_service_rate;

    	char ch;
    	
    	cout<<"For Case A enter A/a \nFor Case B enter B/b \nFor Case C enter C/c \n";
    
    	cin>>ch;
    
    	if(ch == 'A' || ch == 'a')
   	{
   		//cout<<mean_arrival_rate <<"
    		if(mean_arrival_rate >= mean_service_rate)
    		{
    			ofstream MyFile("output.txt");
    			cout<<"The system is unstable\n";
    			MyFile<<"The system is unstable\n";
    			MyFile.close();
    		}
    		else
    		{	
    			cout<<"Simulating........................................................\n";
    			caseA(mean_arrival_rate,mean_service_rate);
    		}
    	}
    	else if(ch == 'B' || ch == 'b')
    	{
    	
    		// no of officers is 2
    	
    		if(mean_arrival_rate >= 2*mean_service_rate)
    		{
    			ofstream MyFile("output.txt");
    			cout<<"The system is unstable\n";
    			MyFile<<"The system is unstable\n";
    			MyFile.close();
    		}
    		else
    		{	
    			cout<<"Simulating........................................................\n";
    			caseB(mean_arrival_rate,mean_service_rate);
    		}

    	}
    	else if(ch == 'C' || ch == 'c')
    	{
    	
    		mean_arrival_rate = mean_arrival_rate*(1-probability);
    		if(mean_arrival_rate >= mean_service_rate)
    		{
    			ofstream MyFile("output.txt");
    			cout<<"The system is unstable\n";
    			MyFile<<"The system is unstable\n";
    			MyFile.close();
    		}
    		else
    		{		
    			cout<<"Simulating........................................................\n";
    			caseC(mean_arrival_rate,mean_service_rate);
    		}
    	}
    	else
    	{
    		cout<<"Please enter the correct entry\n";
    		break;
    	}
    }
   	

}



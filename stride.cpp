#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <vector>

using namespace std;

struct job{
    string name;
    int pass;
    int priority;
    int stride;
    string status;
};

bool systemIsIdle(vector<job> jobs){//system is idle if there are no running and no runnable jobs
    vector<job> runnableJobs;

    for(int i = 0; i < jobs.size(); i++){
        if(jobs.at(i).status == "runnable" || jobs.at(i).status == "running"){
            runnableJobs.push_back(jobs.at(i));
        }
    }

    return runnableJobs.size() == 0;
}

bool sortingComparator(job A, job B){
    //return true = A goes before B, vice versa
    if(A.pass < B.pass){
        return true;
    }
    else if(B.pass < A.pass){
        return false;
    }
    else{
        string tempA = A.name;
        string tempB = B.name;
        transform(tempA.begin(), tempA.end(),tempA.begin(), ::toupper);
        transform(tempB.begin(), tempB.end(),tempB.begin(), ::toupper);
        if(tempA < tempB)
            return true;
        else
            return false;
    }
}

int passOfLastJobRan = 0;

job scheduleDecision(vector<job>& jobs){
    vector<job> runnables;
    for(int i = 0; i < jobs.size(); i++)
        if(jobs.at(i).status == "runnable")
            runnables.push_back(jobs.at(i));

    

    sort(runnables.begin(), runnables.end(), sortingComparator); //sorts the runnables list in order that they will be scheduled

    string nextToRunName = runnables.at(0).name; //names are assumed to be unique

    for(int i = 0; i < jobs.size(); i++){
        if(jobs.at(i).name == nextToRunName){
            jobs.at(i).status = "running";
            passOfLastJobRan = jobs.at(i).pass;
            return jobs.at(i);
        }
    }

}

int main(int argc, char * argv[]) {
    const int large_number = 100000; //expample output seems to imply that this value is 100,000
    vector<job> jobs;

	if(argc != 2){
        cerr << "Error, specify input file" << endl;
        return 1;
    }
    ifstream inputFile(argv[1]);
    if(inputFile.is_open()){
        //do some shit
        string fileLine;
        while(getline(inputFile, fileLine)){
            if(fileLine.at(fileLine.length() - 1) == '\r')
                fileLine = fileLine.substr(0, fileLine.length() - 1); //get rid of \r at the end of every line if there is one
            
            if (fileLine == ""); //skip empty lines
            else{
                if(fileLine == "runnable"){
                    cout << "Runnable:" << endl;

                    vector<job> runnables;
                    for(int i = 0; i < jobs.size(); i++)
                        if(jobs.at(i).status == "runnable")
                            runnables.push_back(jobs.at(i));

                    if(runnables.size() == 0){
                        cout << "None" << endl;
                    }
                    else{
                        cout << setw(8) << left << "NAME"<< setw(8) << left << "STRIDE" << setw(6) << left << "PASS"  << left << "PRI" << endl;
                        
                        sort(runnables.begin(), runnables.end(), sortingComparator);

                        for(int i = 0; i < runnables.size(); i++)
                            cout << setw(8) << left << runnables.at(i).name << setw(8) << left <<  runnables.at(i).stride << setw(6) << left << runnables.at(i).pass << left << runnables.at(i).priority << endl;
                    }
                }
                else if(fileLine == "interrupt"){
                    if(systemIsIdle(jobs))
                        cout << "Error. System is idle." << endl;
                    else{
                        for(int i = 0; i < jobs.size(); i++){
                            if(jobs.at(i).status == "running"){
                                jobs.at(i).pass += jobs.at(i).stride;
                                jobs.at(i).status = "runnable";
                                break;
                            }
                        }

                        job runningJob = scheduleDecision(jobs);

                        cout << "Job: " << runningJob.name << " scheduled." << endl;
                    }
                }
                else if(fileLine == "finish"){
                    if(systemIsIdle(jobs))
                        cout << "Error. System is idle." << endl;
                    else{
                        job previousRunningJob;
                        for(int i = 0; i < jobs.size(); i++)
                            if(jobs.at(i).status == "running"){ //does not need to increment the pass value because we are about to delete the job anyways 
                                previousRunningJob = jobs.at(i);
                                jobs.erase(jobs.begin() + i);
                                break;
                            }

                        cout << "Job: " << previousRunningJob.name << " completed." << endl;

                        if(systemIsIdle(jobs)){
                            cout << "System is idle." << endl;
                        }
                        else{
                            job runningJob = scheduleDecision(jobs);
                            cout << "Job: " << runningJob.name << " scheduled." << endl;
                        }
                    }
                }
                else if(fileLine == "block"){
                    if(systemIsIdle(jobs))
                        cout << "Error. System is idle." << endl;
                    else{
                        job previousRunningJob;
                            for(int i = 0; i < jobs.size(); i++)
                                if(jobs.at(i).status == "running"){ 
                                    previousRunningJob = jobs.at(i);
                                    jobs.at(i).status = "blocked";
                                    break;
                                }

                        cout << "Job: " << previousRunningJob.name << " blocked." << endl;

                        if(systemIsIdle(jobs)){
                            cout << "System is idle." << endl;
                        }
                        else{
                            job runningJob = scheduleDecision(jobs);
                            cout << "Job: " << runningJob.name << " scheduled." << endl;
                        }
                    }
                }
                else if(fileLine == "blocked"){
                    cout << "Blocked:" << endl;
                    
                    vector<job> blocked;
                    for(int i = 0; i < jobs.size(); i++)
                        if(jobs.at(i).status == "blocked")
                            blocked.push_back(jobs.at(i));

                    if(blocked.size() == 0){
                        cout << "None" << endl;
                    }
                    else{
                        cout << setw(8) << left << "NAME"<< setw(8) << left << "STRIDE" << setw(6) << left << "PASS"  << left << "PRI" << endl;

                        for(int i = 0; i < blocked.size(); i++)
                            cout << setw(8) << left << blocked.at(i).name << setw(8) << left <<  blocked.at(i).stride << setw(6) << left << blocked.at(i).pass << left << blocked.at(i).priority << endl;
                    }
                }
                else if(fileLine == "running"){
                    cout << "Running:" << endl;

                    if(systemIsIdle(jobs)){
                        cout << "None" << endl;
                    }
                    else{
                        job runningJob;
                        for(int i = 0; i < jobs.size(); i++){
                            if(jobs.at(i).status == "running"){
                                runningJob = jobs.at(i);
                                break;
                            }
                        }

                        cout << setw(8) << left << "NAME"<< setw(8) << left << "STRIDE" << setw(6) << left << "PASS"  << left << "PRI" << endl;
                        cout << setw(8) << left << runningJob.name << setw(8) << left <<  runningJob.stride << setw(6) << left << runningJob.pass << left << runningJob.priority << endl;
                    }
                }
                else if(fileLine.find(',') != string::npos){ //contains a comma
                    stringstream ssLine(fileLine);
                    vector<string> commandArgs;
                    while(ssLine.good()){
                        string substr;
                        getline(ssLine, substr, ',');
                        commandArgs.push_back(substr);
                    }

                    if(commandArgs.at(0) == "newjob"){
                        vector<job> runnableJobs;
                        int minPass;


                        job newjob;
                        newjob.name = commandArgs.at(1);

                        for(int i = 0; i < jobs.size(); i++)
                            if(jobs.at(i).status == "runnable" || jobs.at(i).status == "running")
                                runnableJobs.push_back(jobs.at(i));
                
                        for(int i = 0; i < runnableJobs.size(); i++){
                            if(i == 0){
                                minPass = runnableJobs.at(i).pass;
                            }
                            else{
                                if(runnableJobs.at(i).pass < minPass)
                                    minPass = runnableJobs.at(i).pass;
                            }
                        }

                        newjob.pass = runnableJobs.size() == 0 ? passOfLastJobRan : minPass;
                        newjob.priority = stoi(commandArgs.at(2));
                        newjob.stride = large_number / newjob.priority;

                        cout << "New job: " << commandArgs.at(1) << " added with priority: " << commandArgs.at(2) << endl; 

                        if(systemIsIdle(jobs)){
                            newjob.status = "runnable";
                            jobs.push_back(newjob);

                            job runningJob = scheduleDecision(jobs);

                            cout << "Job: "<< runningJob.name << " scheduled." << endl;
                        }
                        else{
                            newjob.status = "runnable";
                            jobs.push_back(newjob);
                        }
                    }
                    else if(commandArgs.at(0) == "unblock"){
                        int blockedJobIndex;
                        bool isBlocked = false;
                        for(int i = 0; i < jobs.size(); i++){
                            if(jobs.at(i).name == commandArgs.at(1) && jobs.at(i).status == "blocked"){
                                isBlocked = true;
                                blockedJobIndex = i;
                                break;
                            }
                        }

                        if(isBlocked){
                            int minPass;
                            vector<job> runnableJobs;

                            for(int i = 0; i < jobs.size(); i++)
                                if(jobs.at(i).status == "runnable" || jobs.at(i).status == "running")
                                    runnableJobs.push_back(jobs.at(i));
                
                            for(int i = 0; i < runnableJobs.size(); i++){
                                if(i == 0){
                                    minPass = runnableJobs.at(i).pass;
                                }
                                else{
                                    if(runnableJobs.at(i).pass < minPass)
                                        minPass = runnableJobs.at(i).pass;
                                }
                            }

                            bool systemWasIdle = systemIsIdle(jobs); //need to do this before updating status, else system would detect the recently unblocked job as a runnable

                            jobs.at(blockedJobIndex).pass = runnableJobs.size() == 0 ? passOfLastJobRan : minPass;
                            jobs.at(blockedJobIndex).status = "runnable";

                            cout << "Job: "<< jobs.at(blockedJobIndex).name << " has unblocked. Pass set to: " << jobs.at(blockedJobIndex).pass << endl;

                            if(systemWasIdle){
                                job runningJob = scheduleDecision(jobs);
                                cout << "Job: "<< runningJob.name << " scheduled." << endl;
                            }
                        }
                        else{
                            cout << "Error. Job: " << commandArgs.at(1) << " not blocked." << endl;
                        }
                    }
                }
            }
        }
        inputFile.close();
    }
    else{
        perror("Error");
        return 1;
    }
	return 0;
}

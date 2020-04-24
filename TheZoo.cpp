#include <iostream>
#include <jni.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include "animal.h"

#include "oviparous.h"
#include "mammal.h"

#include "crocodile.h"
#include "goose.h"
#include "pelican.h"

#include "bat.h"
#include "whale.h"
#include "sealion.h"

using namespace std;


vector<Animal*> animalData; 


void GenerateData()               //DO NOT TOUCH CODE IN THIS METHOD
{
     JavaVM *jvm;                      // Pointer to the JVM (Java Virtual Machine)
     JNIEnv *env;                      // Pointer to native interface
                                                              //================== prepare loading of Java VM ============================
     JavaVMInitArgs vm_args;                        // Initialization arguments
     JavaVMOption* options = new JavaVMOption[1];   // JVM invocation options
     options[0].optionString = (char*) "-Djava.class.path=";   // where to find java .class
     vm_args.version = JNI_VERSION_1_6;             // minimum Java version
     vm_args.nOptions = 1;                          // number of options
     vm_args.options = options;
     vm_args.ignoreUnrecognized = false;     // invalid options make the JVM init fail
                                                                          //=============== load and initialize Java VM and JNI interface =============
     jint rc = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);  // YES !!
     delete options;    // we then no longer need the initialisation options.
     if (rc != JNI_OK) {
            // TO DO: error processing...
            cin.get();
            exit(EXIT_FAILURE);
     }
     //=============== Display JVM version =======================================
     cout << "JVM load succeeded: Version ";
     jint ver = env->GetVersion();
     cout << ((ver >> 16) & 0x0f) << "." << (ver & 0x0f) << endl;

     jclass cls2 = env->FindClass("ZooFileWriter");  // try to find the class
     if (cls2 == nullptr) {
            cerr << "ERROR: class not found !";
     }
     else {                                  // if class found, continue
            cout << "Class MyTest found" << endl;
            jmethodID mid = env->GetStaticMethodID(cls2, "createZooFile", "()V");  // find method
            if (mid == nullptr)
                   cerr << "ERROR: method void createZooFile() not found !" << endl;
            else {
                   env->CallStaticVoidMethod(cls2, mid);                      // call method
                   cout << endl;
            }
     }


     jvm->DestroyJavaVM();
     cin.get();
}

string padLeft(string str, int leng, string stringpadVal) {
    for (int i = str.length(); i <= leng; i++){
        str = str + stringpadVal;
    }
    return str;
}

string padRight(string str, int leng, string stringpadVal) {
    for (int i = str.length(); i <= leng; i++)
        str = str + stringpadVal;
    return str;
}   

void addDataToVector(string tNumber, string animalName, string animalType, string animalSubType, int eggs, int nurse){
    Animal *animal; // animal pointer
    if(animalSubType == "Crocodile") {
        Crocodile *c = new Crocodile(tNumber, animalName, animalType, eggs, animalSubType);
        animal = c;
    }
    else if(animalType == "Goose") {
        Goose *g = new Goose(tNumber, animalName, animalType, eggs, animalSubType);
        animal = g;
    }
    else if(animalType == "Pelican") {
        Pelican *p = new Pelican(tNumber, animalName, animalType, eggs, animalSubType);
        animal = p;
    }
    else if(animalType == "Bat") {
        Bat *b = new Bat(tNumber, animalName, animalType, nurse, animalSubType);
        animal = b;
    }
    else if(animalType == "Whale") {
        Whale *w = new Whale(tNumber, animalName, animalType, nurse, animalSubType);
        animal = w;
    }
    else {
        SeaLion *s = new SeaLion(tNumber, animalName, animalType, nurse, animalSubType);
        animal = s;
    }

    animalData.push_back(animal);//a[0] = 2;
}

void AddAnimal() // The following lines of code ask for user input. 
{
    cout << "Add Animal data" << endl;
    
    cout  << "What is the tracking number" << endl;
    string tNumber;
    cin >> tNumber;

    cout << "What is the name" << endl;
    string animalName;
    cin >> animalName;

    cout << "What is the type" << endl;
    string animalType;
    cin >> animalType;

    cout << "What is the subType" << endl;
    string animalSubType;
    cin >> animalSubType;

    cout << "Number of eggs" << endl;
    int eggs;
    cin >> eggs;

    cout << "Require nursing or not, 1 for yes and 0 for no" << endl;
    int nurse;
    cin >> nurse;

    cout << "Add new animal? 1 for confirm and 0 for cancel" << endl;
    int confirm;
    cin >> confirm;

    if(confirm){
        addDataToVector(tNumber, animalName, animalType, animalSubType, eggs, nurse);
        cout << "New animal added";
    } else {
        cout << "Animal discarded";
    }
}


void RemoveAnimal()
{
    cout << "Removing Animal data" << endl;
    cout << "What is the tracking number" << endl;
    string tNumber;
    cin >> tNumber;

    int elementPosition = -1;
    for(int i = 0; i < animalData.size(); i++) {
        if(animalData[i]->getTrackNumber().compare(tNumber) == 0) {
            //[10,20,50,40,30] -> 40
            elementPosition = i; // 4
            break;
        }
    }

    if(elementPosition != -1) { 
        animalData.erase(animalData.begin() + elementPosition);
        cout << "Animal removed";
    } else {
        cout << "Animal not found, hence cant be removed";
    }

}

void LoadDataFromFile()
{
    cout << "Loading Animal data from zoodata.txt" << endl;
    ifstream inputFile("zoodata.txt"); // file handler
    string line; 
    while (getline(inputFile, line)) {
        //line = "0012345 asdfghjk         Oviparous        Crocodile        5 0"
        // ["0012345", "asdfghjk", "Oviparous", "Crocodile", "5", "0"]
        istringstream ss(line); // breaking lines into words
        int fieldCnt = 0;

        string tNumber;
        string animalName;
        string animalType;
        string animalSubType;
        int eggs;
        int nurse;

        do { 
            if(fieldCnt % 6 == 0) {
                ss >> tNumber; //0012345
            }
            else if(fieldCnt % 6 == 1) {
                ss >> animalName; //asdfghjk
            }
            else if(fieldCnt % 6 == 2) {
                ss >> animalType; //Oviparous
            }
            else if(fieldCnt % 6 == 3) {
                ss >> animalSubType; //Crocodile
            }
            else if(fieldCnt % 6 == 4) {
                ss >> eggs; //5
            }
            else {
                ss >> nurse; // 0 

                //cout << tNumber << " " << animalName << " " << animalType << " " << animalSubType << " " << eggs << " " << nurse << endl;
                addDataToVector(tNumber, animalName, animalType, animalSubType, eggs, nurse);
            }

            fieldCnt++; //1
        } while (ss); 
    }

    cout << "Loading Data complete!!" << endl;

}

void SaveDataToFile()
{
    cout << "Saving Animal data" << endl;
    string fileNameToSave;
    cout << "Enter file name to save data";
    cin >> fileNameToSave;

    ofstream outputFile(fileNameToSave); // output to file
    if(outputFile.is_open()) { // create the file
        for(int i = 0; i < animalData.size(); i++) {
            outputFile << (animalData[i]->getTrackNumber() + " " + animalData[i]->getName() + " " +   animalData[i]->getType() + " " +   
             animalData[i]->getSubType() + " " + to_string(animalData[i]->getNumberOfEggs()) + " " +  to_string(animalData[i]->getNurse())) << endl;
        }
        outputFile.close();
    }
}

void DisplayAnimal() {
    cout << "Displaying Animal data" << endl;
    /*
    int i = 0;
    while(i < animalData.size()) {
        //cout << animalData[i]->getTrackNumber() << " " << animalData[i]->getName() << " " << animalData[i]->getType() << " " <<   
         animalData[i]->getSubType() << " " <<    animalData[i]->getNumberOfEggs() << " " <<    animalData[i]->getNurse() << " " <<  endl; 
        i++;
    }
    */
    for(int i = 0; i < animalData.size(); i++) { //a = [1,2,3,4,5]
        //a[0] = 1, a[1] = 2
        cout << animalData[i]->getTrackNumber() << " " << animalData[i]->getName() << " " << animalData[i]->getType() << " " <<   
         animalData[i]->getSubType() << " " <<    animalData[i]->getNumberOfEggs() << " " <<    animalData[i]->getNurse() << " " <<  endl; 
    }
}

void DisplayMenu()
{
    int continueDisplayingMenu = 1;

    while(continueDisplayingMenu) { // whle(0)
        cout << "*********************************" << endl;// "\n" is cleaner but endl is faster respectfully
        cout << "1. Load Animal Data" << endl;
        cout << "2. Generate Data" << endl;
        cout << "3. Display Animal Data" << endl;
        cout << "4. Add Record" << endl;
        cout << "5. Delete Record" << endl;
        cout << "6. Save Animal Data" << endl;
        cout << "*********************************" << endl;

        cout << "Choose option: " << endl;
        int option = -1;
        cin >> option;

        switch(option) { // Switch for Menu Display
            case 1: LoadDataFromFile();
            break;
            case 2: GenerateData(); // creating zoodata.txt using java
            break;
            case 3: DisplayAnimal(); // show whatever in vector
            break;
            case 4: AddAnimal();
            break;
            case 5: RemoveAnimal();
            break;
            case 6: SaveDataToFile();
            break;

        }

        cout << "Do you want to continue? 1 for yes, 0 for no" << endl;
        cin >> continueDisplayingMenu; //continueDisplayingMenu = 0
    }

}

int main()
{
	GenerateData();
	return 1;
}

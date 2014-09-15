/*
 * Singleton.h
 *
 *  Created on: 15.09.2014
 *      Author: Daniel
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_

#include <iostream>

using namespace std;

template <typename C>

/** \brief singleton pattern
 *
 * a singleton class can only exist once
 */
 class Singleton {

 public:
    static C* getInstance (){

       if (!_instance)
          _instance = new C ();
       return _instance;
    }

    virtual ~Singleton () {
       _instance = 0;
    }

 private:
    static C* _instance;

 protected:
    Singleton () { }
 };

 template <typename C> C* Singleton <C>::_instance = 0;



/** \brief singleton sample
 *
 * sample implementation of Singleton as reference
 */
class ConcreteSingleton : public Singleton <ConcreteSingleton> {
 friend class Singleton <ConcreteSingleton>;

 public:
    ~ConcreteSingleton () { }
    void helloWorld () {cout<<"Hey Welt, Ich bin einzige Instanz von ConcreteSingleton!"<<endl; }

 protected:
    ConcreteSingleton () { }
 };

#endif /* SINGLETON_H_ */

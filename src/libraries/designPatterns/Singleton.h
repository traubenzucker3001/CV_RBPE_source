
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
	/** \brief getter
	 *
	 * get the only instance of the corresponding class
	 */
    static C* getInstance (){

       if (!instance)
          instance = new C ();
       return instance;
    }

	/** \brief destructor
	 *
	 * sets the only instance to 0
	 */
    virtual ~Singleton () {
       instance = 0;
    }

 private:
    static C* instance;		/**< own instance of the corresponding class */

 protected:
	/** \brief constructor
	 *
	 * creates the only instance of the class if not already existing
	 */
    Singleton () { }
 };

 template <typename C> C* Singleton <C>::instance = 0;



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

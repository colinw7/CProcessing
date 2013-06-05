#ifndef CTHREAD_H
#define CTHREAD_H

#include <ostream>
#include <vector>
#include <list>

#include "pthread.h"

extern "C" {
#include <pthread_rdwr.h>
}

#include <CThrow.h>

typedef void *(*CThreadProc)(void *data);

class CThreadArrayProc;
class CThreadArray;
class CThread;
class CThreadMutex;
class CThreadCondition;

class CThreadOnce {
 public:
  typedef void (*OnceProc)();

 private:
  pthread_once_t once_;
  OnceProc       proc_;

 public:
  CThreadOnce(OnceProc proc);

  void exec();
};

// TODO: pthread_cleanup_push and pthread_cleanup_pop are macros !

#if 0
class CThreadCleanup {
 private:
  int exec_flag_;

 public:
  CThreadCleanup(void (*proc)(void *), void *data) :
   exec_flag_(0) {
    pthread_cleanup_push(proc, data);
  }

 ~CThreadCleanup() {
    pthread_cleanup_pop(exec_flag_);
  }

  void setExecFlag(bool flag) {
    exec_flag_ = (flag ? 1 : 0);
  }
};
#endif

template<typename T>
class CThreadKeyData {
 private:
  pthread_key_t key_;

 public:
  CThreadKeyData() {
    pthread_key_create(&key_, (void *) free_key);
  }

 ~CThreadKeyData() {
    pthread_key_delete(&key_);
  }

  T getValue() {
    T *pvalue;

    pvalue = pthread_getspecific(key_);

    if (pvalue == NULL) {
      CTHROW("No Data for Key");
      return T();
    }

    return *pvalue;
  }

  void setValue(const T &value) {
    T *pvalue;

    pvalue = pthread_getspecific(key_);

    if (pvalue == NULL) {
      pvalue = new T;

      pthread_setspecific(key_, (void *) pvalue);
   }

    *pvalue = value;
  }

 private:
  static void free_key(void *pvalue) {
    delete pvalue;
  }
};

class CThreadArray {
 private:
  static bool debug_;

  int                              n_;
  int                              num_active_;
  CThreadMutex                    *thread_mutex_;
  CThreadMutex                    *alive_mutex_;
  CThreadCondition                *thread_cond_;
  std::vector<CThreadArrayProc *>  procs_;
  std::list<CThread *>             alive_threads_;
  std::list<CThread *>             dead_threads_;

 public:
  static void setDebug(bool debug);

  CThreadArray(int n);
 ~CThreadArray();

  bool startThread(CThreadProc proc, void *data = NULL);
  bool startThread(CThreadArrayProc *proc);

  bool endThread(CThread *thread);

  bool join(void **data = NULL);

  static void *executeCB(void *data);

 private:
  bool startThread1(CThreadArrayProc *proc);

  void addArrayProc(CThreadArrayProc *array_proc);
  void deleteArrayProcs();
  bool joinDeadThreads(void **data);
  bool getDeadThreads(CThread ***dead_threads, int *num_dead_threads);

  void addAliveThread(CThread *thread);
  void removeAliveThread(CThread *thread);
  bool joinAliveThreads(void **data);
  bool getAliveThreads(CThread ***alive_threads, int *num_alive_threads);
};

class CThreadArrayProc {
 protected:
  CThreadArray *array_;
  CThread      *thread_;

 public:
  CThreadArrayProc(CThreadArray *array) :
   array_(array), thread_(NULL) {
  }

  virtual ~CThreadArrayProc() { }

  CThread *getThread() const { return thread_; }

  void setThread(CThread *thread) { thread_ = thread; }

  void *executeCB();

  virtual void *execute() { return NULL; }
};

class CThreadArrayCProc : public CThreadArrayProc {
 private:
  CThreadProc  proc_;
  void        *data_;

 public:
  CThreadArrayCProc(CThreadArray *array, CThreadProc proc, void *data = NULL) :
   CThreadArrayProc(array), proc_(proc), data_(data) {
  }

  void *execute();
};

class CThread {
 private:
  pthread_t thread_;

  static pthread_attr_t attr_;
  static bool           attr_inited_;

  static int  num_threads_;
  static bool debug_;

 public:
  static void setDebug(bool debug) { debug_ = debug; }

  CThread();
  CThread(pthread_t thread);

  virtual ~CThread();

  bool start();
  bool start(CThreadProc proc, void *data = NULL);

  bool join(void **data = NULL);
  bool detach();

  bool cancel();

  static void testcancel();

  bool kill(int signal);

  bool setCancelState(int state);
  bool setCancelType(int type);

  bool operator==(const CThread &thread);

  virtual void *execute() { return NULL; }

  static void exit(void *data);

  static bool setDetachState(int state);
  static bool getDetachState(int *state);
  static bool setScope(int scope);
  static bool getScope(int *scope);
  static bool setStack(void *addr, unsigned int size);
  static bool getStack(void **addr, unsigned int *size);
  static bool setSchedulePolicy(int policy, int priority = -1);
  static bool getSchedulePolicy(int *policy);
  static bool setInheritSchedule(int inherit = PTHREAD_INHERIT_SCHED);
  static bool getInheritSchedule(int *inherit);

  static int getMinPriority(int policy);
  static int getMaxPriority(int policy);
  static int getAvgPriority(int policy);

  bool updateSchedulePolicy(int policy, int priority);
  bool getSchedulePolicy(int *policy, int *priority);

  static bool blockSignals(int signum, ...);

  static bool atFork(void (*prepareProc)(),  void (*parentProc)(), void (*childProc)());

  static pthread_t getSelf();

  static void testCancel();

 private:
  static pthread_attr_t *getAttr();
  static void            resetAttr();

  static void *executeCB(void *data);
};

class CThreadMutex {
 private:
  const char      *id_;
  pthread_mutex_t  mutex_;
  bool             locked_;

  static pthread_mutexattr_t attr_;
  static bool                attr_inited_;

  static bool debug_;

 public:
  CThreadMutex(const char *id = NULL);
 ~CThreadMutex();

  static void setDebug(bool debug) { debug_ = debug; }

  bool lock();
  bool try_lock();
  bool unlock();

  pthread_mutex_t *getMutexP() { return &mutex_; }

  bool locked() const { return locked_; }

  static bool setPriorityProtect();
  static bool setPriorityInherit();

  static bool getProtocol(int *protocol);
  static bool setProtocol(int protocol);

  static bool getPriorityCeiling(int *ceiling);
  static bool setPriorityCeiling(int ceiling);

  static bool getProcessShared(int *shared);
  static bool setProcessShared(int shared);

 private:
  static pthread_mutexattr_t *getAttr();
  static void                 resetAttr();
};

template<typename T>
class CThreadMutexVar {
 private:
  T            &value_;
  CThreadMutex  mutex_;

 public:
  CThreadMutexVar(T value) : value_(value) { }
 ~CThreadMutexVar() { }

  void set(T value) {
    mutex_.lock();

    value_ = value;

    mutex_.unlock();
  }

  T get() {
    mutex_.lock();

    T value1 = value_;

    mutex_.unlock();

    return value1;
  }

  void incr() {
    mutex_.lock();

    ++value_;

    mutex_.unlock();
  }

  void decr() {
    mutex_.lock();

    --value_;

    mutex_.unlock();
  }

 private:
  CThreadMutexVar(CThreadMutexVar &var);

  CThreadMutexVar &operator=(CThreadMutexVar &var);
};

class CThreadAutoMutex {
 private:
  CThreadMutex *mutex_;
  bool          alloc_;

 public:
  CThreadAutoMutex(CThreadMutex *mutex = NULL) :
   mutex_(mutex), alloc_(false) {
    if (mutex_ == NULL) {
      mutex_ = new CThreadMutex;
      alloc_ = true;
    }

    mutex_->lock();
  }

 ~CThreadAutoMutex() {
    mutex_->unlock();

    if (alloc_)
      delete mutex_;
  }

  CThreadMutex *getMutex() {
    return mutex_;
  }
};

#ifdef _POSIX_THREAD_PROCESS_SHARED
class CThreadSharedMutex {
 private:
  int              shared_mem_id_;
  void            *shared_mem_ptr_;
  pthread_mutex_t *mutex_;
  bool             locked_;

 public:
  CThreadSharedMutex();
 ~CThreadSharedMutex();

  bool lock();
  bool try_lock();
  bool unlock();

  pthread_mutex_t *getMutexP() { return mutex_; }

  bool locked() const { return locked_; }
};
#endif

class CThreadReadWriteMutex {
 private:
  pthread_rdwr_t mutex_;
  bool           read_locked_;
  bool           write_locked_;

 public:
  CThreadReadWriteMutex();
 ~CThreadReadWriteMutex();

  bool read_lock();
  bool write_lock();
  bool read_unlock();
  bool write_unlock();

  bool read_locked() const { return read_locked_; }
  bool write_locked() const { return write_locked_; }
};

class CThreadCondition {
 private:
  const char     *id_;
  pthread_cond_t  cond_;

  static pthread_condattr_t attr_;
  static bool               attr_inited_;

  static bool debug_;

 public:
  CThreadCondition(const char *id = NULL);
 ~CThreadCondition();

  static void setDebug(bool debug) { debug_ = debug; }

  bool wait(CThreadMutex &mutex);
  bool timed_wait(CThreadMutex &mutex, struct timespec *time);

  bool signal();
  bool broadcast();

  static bool getProcessShared(int *shared);
  static bool setProcessShared(int shared = PTHREAD_PROCESS_SHARED);

 private:
  static pthread_condattr_t *getAttr();
  static void                resetAttr();
};

// TODO: CThreadSharedCondition

template<typename T>
class CThreadVarCondition {
 private:
  T                &value_;
  CThreadMutex      mutex_;
  CThreadCondition  condition_;
  bool              signalled_;

 public:
  CThreadVarCondition(T &value) :
   value_(value), signalled_(false) {
  }

 ~CThreadVarCondition() { }

  bool check() {
    return value_.check();
  }

  bool lock()     { return mutex_.lock(); }
  bool try_lock() { return mutex_.try_lock(); }
  bool unlock()   { return mutex_.unlock(); }

  void wait() {
    if (! mutex_.locked())
      std::cerr << "condition wait when not locked" << std::endl;

    condition_.wait(mutex_);
  }

  void timed_wait(struct timespec *time) {
    if (! mutex_.locked())
      std::cerr << "condition timed_wait when not locked" << std::endl;

    condition_.timed_wait(mutex_, time);
  }

  void signal() {
    if (! mutex_.locked())
      std::cerr << "condition signal when not locked" << std::endl;

    condition_.signal();

    signalled_ = true;
  }

  bool signalled() {
    return signalled_;
  }

  void broadcast() {
    if (! mutex_.locked())
      std::cerr << "condition broadcast when not locked" << std::endl;

    condition_.broadcast();
  }

  void print(std::ostream &os) const {
    os << value_;
  }

  friend std::ostream &operator<<(std::ostream &os, const CThreadVarCondition &c) {
    c.print(os);

    return os;
  }
};

class CThreadFile {
 private:
  FILE *fp_;
  bool  locked_;

 public:
  CThreadFile(FILE *fp) : fp_(fp) { }
 ~CThreadFile();

  bool lock();
  bool try_lock();
  bool unlock();
};

#endif

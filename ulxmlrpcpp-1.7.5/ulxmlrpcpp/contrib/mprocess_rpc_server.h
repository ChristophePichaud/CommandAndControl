/***************************************************************************
             mprocess_rpc_server.h  -  multi process rpc server
                             -------------------
    begin                : Sun May 29 2005
    copyright            : (C) 2005 Dmitry Nizovtsev <funt@alarit.com>
                                    Olexander Shtepa <isk@alarit.com>

    $Id: mprocess_rpc_server.h 945 2007-01-03 13:05:48Z ewald-arnold $

 ***************************************************************************/

/**************************************************************************
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 ***************************************************************************/


#ifndef FUNTIK_MPROCESS_RPC_SERVER_H
#define FUNTIK_MPROCESS_RPC_SERVER_H

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

//STL
#include <vector>
#include <map>
#include <memory>

#include <ulxmlrpcpp/ulxr_dispatcher.h>
#include <ulxmlrpcpp/ulxr_tcpip_connection.h>

#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include <signal.h>


namespace funtik {


class ULXR_API_DECL0 MultiProcessRpcServerError : public std::exception
{
	std::string _what;
public:
	MultiProcessRpcServerError(const std::string& what_arg);
	~MultiProcessRpcServerError() throw();
	const char* what () const throw();
};

 /**
  *  @brief multi process handler for RPC requests.
  *
  *  This create separate process for every request.
  *  Have some wrappers for dispatcher methods(example AddMethods).
  * @ingroup grp_ulxr_rpc
  */

class ULXR_API_DECL0 MultiProcessRpcServer
{
public:
    struct ProcessContext
    {
//    	pid_t	pidProcess;
    	time_t	timeStart;
    	int 	iState;
    };

  /**
    * @brief Constructs the rpc server.
    *
    * Constructs the rpc server. It is default constructor.
	* NOTE: you must set bHandleSigchld=false only if you handle signals manually.
	*
    * @param uNumProcess   	number of max process
    * @param bHandleSigchld true: handle signal sigchild
   	* @param wbxml_mode  	true: data is sent as wbxml
	* @sa sigchild_handler().
    */
   MultiProcessRpcServer(unsigned uNumProcess=0,bool bHandleSigchld=true, bool wbxml_mode = false);

  /**
    * @brief Constructs the rpc server.
   	*
   	* Constructs the rpc server.
   	*
	* NOTE: you must set bHandleSigchld=false only if you handle signals manually.
	*
    * @param poProtocol 	Protocol object
    * @param uNumProcess   	number of max process
  	* @param bHandleSigchld true: handle signal sigchild
   	* @param wbxml_mode  	true: data is sent as wbxml
	*
    */
    MultiProcessRpcServer(ulxr::Protocol* poProtocol, unsigned uNumProcess,bool bHandleSigchld=true, bool wbxml_mode = false);


  /** @brief Destructs the rpc server.
   *
   *  Destructs the rpc server. The all connection will be closed.
   *  All children will be killed.
   *
   */
	virtual ~MultiProcessRpcServer();

  /**
   * @brief  Gets the number of started processes.
   *
   *  Gets the number of started processes.
   *
   *  @return number of processes
   */
    virtual unsigned getNumProcesses() const;

  /** @brief Terminates all process.
   *
   * The process only get a signal and must terminate themselves!
   * After this commands the new processes will not created.
   * Future version might include a way to kill the threads with brute force
   * after some grace time.
   *
   * @param bForce true if need kill all proccesses, after lTimeout wait.
   * @param lTimeout time in ms to wait finished of process .
   * @return true if all process was finished, else return false.
   */
    virtual bool terminateAllProcess(bool bForce,long lTimeout = 3000);

  /**
   * @brief Set max number of executed processes.
   *
   * Set max number of executed processes.
   *
   * @param uNumProcesses
   */
	void setMaxNumProcesses(unsigned uNumProcesses);

  /**
   * @brief Get max number of executed processes.
   *
   * Get max number of executed processes.
   * If number of executed will be more then generate exception.
   *
   * @return max number of executed processes.
   */
	unsigned getMaxNumProcesses() const;


  /**
   * @brief Get dispatcher.
   *
   * The received dispatcher can be initialized,etc.
   * NOTE: don`t delete received dispatcher manually.
   *
   * @return pointer to the dispatcher object.
   */
	ulxr::Dispatcher* getDispatcher() const;


  /**
   * @brief Reset dispather.
   *
   * It  destroy old dispather and create new,empty dispather.
   *
   * @param poProtocol if protocol not 0 then new will be used.
   */
	void resetDispatcher(ulxr::Protocol* poProtocol=0);


  /**
   * @brief Start main server loop
   *
   * The main process function that does the work.
   * If dispatcher don`t set than it generate Exception.
   * serverLoop exit if ServerState = FINISH
   */
	virtual void serverLoop();


  /**
   * @brief The signal handler function.
   *
   * The signal handler function.
   * It is invoke when child finish work.
   *
   * it wait children (via waitpid) and update inner structures.
   *
   * WARNING: if you redefine sighchild_handler in children class then
   * you must call MultiProcessRpcServer::sigchild_handler from your SIGCHILD handler.
   *
   * @param signal signal number
   * @sa sigchild_register().
   */
    static void sigchild_handler(int signal);

 /**
  * @brief handle request.
  *
  * The child process that does the work.
  * Every child process will started in this function.
  * Exceptions:
  * 	ulxr::Exception,ulxr::ConnectionException
  *
  */
	virtual void handleRequest();

 /**
  * @brief wait the connection.
  *
  * It blocked function wait connection and return true if connection received.
  *
  * @return true if exist validate connection, false if bInterruptBySig is true and received signal.
  */
	virtual bool waitConnection(bool bInterruptBySig=true);

 /**
  * @brief Set state of process.
  *
  * Set state of process.
  *
  * @param iState set state.
  */
   	void setState(int iState);


 /**
  * @brief print state of all children process.
  *
  * NOTE: Use this function only for tests.
  *
  */
  	void printProcess() const;

 /**
  * @brief Get current state of process.
  *
  * Get current state of process.
  *
  * @return current state of process.
  */
    int getState() const;


 /**
  * @brief Get current process statistics.
  *
  *
  */
    virtual std::map<pid_t,ProcessContext> getProcInfo() const;

 /**
  * @brief wait finish all child process.
  *
  * @param lTimeout time for wait in miliseconds.
  * @return true if all processes finished else return false.
  */

	virtual bool waitChildren(long lTimeout=-1) const;

    enum ProcessState
    {
    	RUN = 1,
    	FINISH = 2
    };

protected:

 /**
  * @brief Start child process.
  *
  * The child process that does the work.
  * Every child process will started in this function.
  *
  */
	virtual void doChild();

	virtual void acceptConnection();

 /**
  * @brief store or print information about result RPC call.
  *
  * It can be used for store information about call RPC method in log,etc.
  * On default it write info to the console(cout).
  *
  */
	virtual void storeFuncResult(const ulxr::MethodCall& callMethod,const ulxr::MethodResponse& respMethod) const;

 /**
  * @brief Read xml query and parse it.
  *
  *
  */
	virtual ulxr::MethodCall handleXmlRequest();

 /**
  * @brief Block SIGCHLD.
  *
  */
	virtual void blockSigchld();

 /**
  * @brief Unblock SIGCHLD.
  *
  * NOTE: if SIGCHILD was blocked before call blockSigchld().
  *
  * @sa blockSigchld().
  */
	virtual void unblockSigchld();

 /**
  * @brief store information about process.
  */
	virtual void storeProcessData(pid_t pid);

 /**
  * @brief sweep process table.
  *
  * Sweep all complete process.
  *
  */
 	virtual void sweepProcessData();

 /**
  * @brief the register of signal handler function.
  *
  * @sa sigchild_handler(),sigchild_unregister().
  */
	virtual void sigchild_register();

 /**
  * @brief the register of signal handler function.
  *
  * @sa sigchild_handler(),sigchild_register().
  */
	virtual void sigchild_unregister();

	struct sigaction old_sigchld;


	unsigned						m_maxProcesses;
	static std::map<pid_t,ProcessContext>	m_mapProcesses;
	std::auto_ptr<ulxr::Dispatcher>	m_poDispatcher;
	int								m_iState;
	bool							m_wbxml_mode;
private:
	bool							m_bHandleSigchld;
};


} // namespace funtik


#endif // FUNTIK_MPROCESS_RPC_SERVER_H

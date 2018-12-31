/***************************************************************************
                  ulxr_mutex.h  -  mutex handling
                      -------------------
    begin                : Thu Dec 03 2002
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxr_mutex.h 940 2006-12-30 18:22:05Z ewald-arnold $

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

#ifndef ULXR_MUTEX_H
#define ULXR_MUTEX_H

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#ifdef __WIN32__
#include <windows.h>
#endif

#ifdef __unix__
#include <pthread.h>
#endif


namespace ulxr {


/** Supporter class for mutual exclusion of processes/threads.
  */
class ULXR_API_DECL0 Mutex
{
 public:

 /** Helper class to request and release a mutex by pure presence.
   */
   class ULXR_API_DECL0 Locker
   {
    public:

   /** Create object and request mutex.
     * @param mtx  mutex to request.
     */
     Locker(Mutex &mtx);

   /** Destroys object and releases mutex.
     */
     ~Locker() throw();

    private:

      Mutex *mutex;
   };

 /** Helper class to request and release a mutex by pure presence.
   */
   class ULXR_API_DECL0 TryLocker
   {
    public:

   /** Create object and request mutex.
     * @param mtx  mutex to request.
     */
     TryLocker(Mutex &mtx);

   /** Destroys object and releases mutex.
     */
     ~TryLocker() throw();

   /** Checks if the mutex has been succesfully requested.
     * @return false: the mutex was not available
     */
     bool isLocked();

    private:

      Mutex *mutex;
      bool   locked;
   };

 /** Creates the mutex object.
   */
   Mutex();

 /** Destructs mutex object.
   */
   ~Mutex() throw();

 /** Waits for the mutex to become available.
   */
   void lock();

 /** Releases the mutex.
   */
   void  unlock();

 /** Tries to request the mutex.
   * @return false: the mutex was not available
   */
   bool tryLock();

 private:

#ifdef __WIN32__
   HANDLE   handle;
#else
   pthread_mutex_t handle;
#endif
};


}  // namespace ulxr


#endif // ULXR_MUTEX_H

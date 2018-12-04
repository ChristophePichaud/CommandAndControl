/***************************************************************************
         ulxr_cached_resource.h - resources accessed via ulxmlrpcpp
                             -------------------
    begin                : Sun May 1 2005
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxr_cached_resource.h 940 2006-12-30 18:22:05Z ewald-arnold $

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

#ifndef ULXR_CACHED_RESOURCE_H
#define ULXR_CACHED_RESOURCE_H

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header


namespace ulxr {


/** Abstraction of a resource accessed via xmlrpc or http.
  * The data is stored in a stream alike fashion. It is possible
  * to read from and write to the same object. Writing simply appends
  * to the object. Reading is done with a stream pointer.
  * @ingroup grp_ulxr_utilit
  */
class ULXR_API_DECL0 CachedResource
{
  public:

 /** Creates the resource.
   * @param  name  the name of the resource
   */
   CachedResource(const CppString &name);

 /** Destroys the resource.
   */
   virtual ~CachedResource();

 /** Gets the resource name.
   */
   CppString getResourceName() const;

 /** Opens the resource.
   */
   virtual void open() = 0;

 /** Closes the resource.
   */
   virtual void close() = 0;

 /** Tests the state of the resource.
   * @return true: state is ok
   * @return false: an error occured
   */
   virtual bool good() = 0;

 /** Reads the remaining data from the current position.
   * @return remaining data
   */
   std::string read();

 /** Reads a chunk of data the remaining data from the current position.
   * @param len  number of chracters to read
   * @return remaining data
   */
   std::string read(unsigned len);

 /** Appends a chunk of data at the end of the cache string.
   * @param s   string to append
   */
   void write (const std::string &s);

 /** Appends a chunk of data at the end of the cache string.
   * @param str   start of string to append
   * @param len   number of characters to write
   */
   void write (const char *str, unsigned len);

 /** Clears the internal cache and resets the read pointer.
   */
   virtual void clear();

 /** Resets the read pointer to the beginning.
   */
   virtual void reset();

 /** Reads the complete cached data.
   * @return complete data
   */
   std::string data() const;

  private:

   CppString    name;
   std::string  cache;
   unsigned     read_pointer;
};


};  // namespace ulxr


#endif // ULXR_CACHED_RESOURCE_H

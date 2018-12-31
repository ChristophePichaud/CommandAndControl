/***************************************************************************
                  ulxr_call.h  -  create xml-rpc calling data
                             -------------------
    begin                : Sun May 1 2005
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxr_file_resource.h 940 2006-12-30 18:22:05Z ewald-arnold $

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

#ifndef ULXR_FILE_RESOURCE_H
#define ULXR_FILE_RESOURCE_H

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#include <ulxmlrpcpp/ulxr_cached_resource.h>


namespace ulxr {

/** A resource that caches a file.
  * @ingroup grp_ulxr_utilit
  */
class ULXR_API_DECL0 FileResource : public CachedResource
{
  public:

 /** Creates the resource.
   * @param  res_name   the name of the resource
   * @param  file_name  the name of the corresponding file
   * @param  do_read    true: actually read file, otherwise just create internal structures
   */
   FileResource(const CppString &res_name, const CppString &file_name, bool do_read = true);

 /** Destroys the resource.
   */
   virtual ~FileResource();

 /** Opens the resource and loads the file if it exists.
   */
   virtual void open();

 /** Opens the resource and loads the file if it exists.
   * @param  do_read  true: actually read file, otherwise just create internal structures
   */
   virtual void open(bool do_read);

 /** Closes the resource and writes the current cache content
   * to the file.
   */
   virtual void close();

 /** Tests the state of the resource and the success of the
   * transfers from and to the corresponding file.
   * @return true: state is ok
   * @return false: an error occured
   */
   virtual bool good();

 /** Gets the filename.
   */
   CppString getFilename() const;

 /** Clears the cache and removes the associated file.
   * The resource is also closed and has to be reopened for
   * further use.
   */
   virtual void clear();

 /** Resets the read pointer and internal states.
   */
   virtual void reset();

  private:

   CppString  filename;
   bool       error;
   bool       opened;
};


};  // namespace ulxr

#endif // ULXR_FILE_RESOURCE_H

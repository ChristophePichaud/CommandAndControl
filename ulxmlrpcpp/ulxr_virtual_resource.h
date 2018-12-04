/***************************************************************************
   ulxr_virtual_resource.h - virtual resources accessed via ulxmlrpcpp
                             -------------------
    begin                : Sun May 1 2005
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxr_virtual_resource.h 940 2006-12-30 18:22:05Z ewald-arnold $

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

#ifndef ULXR_VIRTUAL_RESOURCE_H
#define ULXR_VIRTUAL_RESOURCE_H

#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#include <ulxmlrpcpp/ulxr_cached_resource.h>


namespace ulxr {


/** A resource that only exists in memory.
  * @ingroup grp_ulxr_utilit
  */
class ULXR_API_DECL0 VirtualResource : public CachedResource
{
  public:

 /** Creates the resource.
   * @param  res_name  the name of the resource
   */
   VirtualResource(const CppString &res_name);

 /** Destroys the resource.
   */
   virtual ~VirtualResource();

 /** Opens the resource.
   */
   virtual void open();

 /** Closes the resource.
   */
   virtual void close();

 /** Tests the state of the resource.
   * @return true: state is ok
   * @return false: an error occured
   */
   virtual bool good();
};


};  // namespace ulxr

#endif // ULXR_VIRTUAL_RESOURCE_H

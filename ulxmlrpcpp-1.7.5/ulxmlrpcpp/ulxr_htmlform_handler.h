/***************************************************************************
        ulxr_htmlform_handler.h  -  work with html pages and forms
                             -------------------
    begin                : Fri Dec 05 2003
    copyright            : (C) 2002-2007 by Ewald Arnold
    email                : ulxmlrpcpp@ewald-arnold.de

    $Id: ulxr_htmlform_handler.h 942 2006-12-31 13:10:54Z ewald-arnold $

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

#ifndef ULXR_HtmlFormHandler_H
#define ULXR_HtmlFormHandler_H


#include <ulxmlrpcpp/ulxmlrpcpp.h>  // always first header

#include <ulxmlrpcpp/ulxr_http_server.h>
#include <ulxmlrpcpp/ulxr_except.h>


namespace ulxr {


/** Helper class to deal with form data.
  * @ingroup grp_ulxr_http
  */
class ULXR_API_DECL0 HtmlFormData
{
  public:
   typedef std::map<CppString, std::vector<CppString> > Elements;

/** Adds another name/value pair from a form.
  * @param name   the name of the form element
  * @param value  the form elements value
  */
   void addElement(const CppString &name, const CppString &value);

/** Gets all the values for a form element.
  * @param name   the name of the form element
  * @return  the form elements values (one or more)
  */
   std::vector<CppString> getElement(const CppString &name) const;

/** Gets all the values for a form element.
  * Intended for numbered elements with the pattern "name" "_" [number]
  * as created by HtmlFormHandler::makeIdent()
  * @param name   the name of the form element
  * @param index  the elements index
  * @return  the form elements values (one or more)
  */
   std::vector<CppString> getElement(const CppString &name, unsigned index) const;

/** Determines if an element is available in the form.
  * @param name   the name of the form element
  * @return  true: at least on values is available
  */
   bool hasElement(const CppString &name) const;

/** Determines if an element is available in the form.
  * Intended for numbered elements with the pattern "name" "_" [number]
  * as created by HtmlFormHandler::makeIdent()
  * @param name   the name of the form element
  * @param index  the elements index
  * @return  true: at least on values is available
  */
   bool hasElement(const CppString &name, unsigned index) const;

/** Gets the number of available elements.
  * @return  number of form elements
  */
   unsigned size() const;

  private:

    Elements  elements;
};


namespace hidden
{

/** Interface class to forward a call to a resource handler.
  */
class ULXR_API_DECL0 SubResourceBase
{
  public:

  /** Creates a handler for a subresource.
    * @param name   name of the subresource
    * @param descr  human readable description for this resource
    */
    SubResourceBase(const CppString &name, const CppString &descr);

  /** Destroy handler.
    */
    virtual ~SubResourceBase();

  /** Call the handler indirectly.
    * @param  formdata  optional data from a form
    * @param  mimetype  \b returns the mimetype of the data
    * @return the html data
    */
    virtual CppString call(const HtmlFormData &formdata, CppString &mimetype) = 0;

  /** Gets the name of the resource.
    * @return name
    */
    CppString getName() const;

  /** Gets the description of the resource.
    * @return description
    */
    CppString getDescription() const;

  private:

    CppString    name;
    CppString    descr;
};


/** Helper class to forward a call to a resource handler.
  */
template<class T>
class SubResource : public SubResourceBase
{
  public:

  /** A handler for a html request.
    * @param  formdata  optional data from a form
    * @param  mimetype  [OUT] \b returns the mimetype of the data
    * @return the html data
    */
  typedef CppString (T::*PMF)(const HtmlFormData &formdata, CppString &mimetype);

  /** Creates a handler for a subresource.
    * @param in_name   of the subresource
    * @param in_obj    pointer of the object which handles it
    * @param in_pmf    pointer of a member function in the object
    * @param in_descr  human readable description for this resource
    */
    SubResource(const CppString &in_name, T* in_obj, PMF in_pmf, const CppString &in_descr)
        : SubResourceBase(in_name, in_descr)
        , obj(in_obj)
        , pmf(in_pmf)
    {}

  /** Call the handler indirectly.
    * @param  formdata  optional data from a form
    * @param  mimetype  \b returns the mimetype of the data
    * @return the html data
    */
    CppString call(const HtmlFormData &formdata, CppString &mimetype)
    {
      return (obj->*pmf)(formdata, mimetype);
    }

  private:
    T        *obj;
    PMF       pmf;
};


} // namespace hidden


/** Helper class to deal with html "post" and "get" requests.
  * Intended to be added to a \c HttpServer object as handler for
  * one or more special resources.
  * All transfered content is expected to be in UTF-8 encoding when unicode is enabled,
  * otherwise you need to take care of your local 8bit encoding (8859-1 as default).
  * @ingroup grp_ulxr_http
  */
class ULXR_API_DECL0 HtmlFormHandler
{
  public:

  /** Construct the form handler.
    * The identifier which starts resource handled here.
    * @param resource  name of the resource that is handled here.
    */
    HtmlFormHandler(const CppString &resource);

  /** Destroys the handler.
    */
    virtual ~HtmlFormHandler();

  /** Extracts the form elements that come within the http request.
    * @param  prot       pointer to the according connection
    * @param  method     the method in use
    * @param  resource   the requested resource
    * @param  conn_data  already received chunk of the message body.
    */
    HtmlFormData extractFormElements(HttpProtocol *prot,
                                     const CppString &method,
                                     const CppString &resource,
                                     const Cpp8BitString &conn_data);

  /** Split an incoming resource into the base part and the extension.
    * This method assumes that the resource consists of two main parts, usually
    * regarded as a "file within a path": a directory-like part (the resource start)
    * and a file-like part (the target).
    * @param  method     the method in use
    * @param resource   request resource from a client
    * @param rsc_start  the main part of the resource which is handled
    * @param target     the special part
    * @return true: resource beginner matches and target contains the remainder
    */
    static bool resourceSplit(const CppString &method,
                              const CppString &resource,
                              const CppString &rsc_start,
                              CppString &target);

  /** Decodes the form data into normal characters.
    * Incoming form data is encoded according to RFC2396.
    * @param value  reference to value
    * @return
    */
    static std::string formDecode(std::string &value);

  /** Encodes the html data into the desired encoding for transmission.
    * @param data  html data from the handler
    * @return encoded data in 8bit characters
    */
    virtual std::string encodeForHtml(const CppString &data);

  /** Encodes the html data for use in the hanlder
    * @param data  html data from the request
    * @return encoded data in ulxr::CppString
    */
    virtual CppString encodeFromHtml(const std::string &data);

  /** Return html data which opens a form.
    * @param name   the form name
    * @param method the http method to submit the form
    * @return the according html sequence
    */
    CppString openForm(const CppString &name, const CppString &method);

  /** Return html data which closes a form.
    * @return the according html sequence
    */
    static CppString closeForm();

  /** Creates an html anchor.
    * @param  url    the url of this anchor
    * @param  descr  the anchor's description
    * @return  the html sequence
    */
    CppString makeAnchor(const CppString &url, const CppString &descr);

  /** Creates an identifier with a number.
    * @param name   the identifiers name
    * @param index  an additional index which is appended
    * @return  the name and index with an underscore between
    */
    static CppString makeIdent(const CppString &name, unsigned index);

  /** Turns a number into a hex string.
    * the string includes the C-style "0x" prefix
    * @param index the number to transform
    * @return  the hex string of the number
    */
    static CppString makeHexNumber(unsigned index);

  /** Turns a number into a decimal string.
    * @param index the number to transform
    * @return  the decimal string of the number
    */
    static CppString makeNumber(unsigned index);

  /** Turns a strin into a long value.
    * @param num the number string
    * @return  the number as long
    */
    static long getLong(const CppString &num);

  /** Make a pice of text hmtl conforming.
    * Dangerous characters like & and < are trasnformed into their html entities.
    * @param text  the text to transform if needed
    * @return  the html conforming string
    */
    static CppString makeText(const CppString &text);

  /** Creates a html sequence for a checkbox.
    * @param name     the checkbox name
    * @param value    the checkbox value
    * @param checked  true: set checked
    * @return  the html sequence
    */
    static CppString makeCheckBox(const CppString &name, const CppString &value,
                                  bool checked = false);

  /** Creates a html sequence for a text field (one single line).
    * @param name   the field name
    * @param value  the initial text
    * @return  the html sequence
    */
    static CppString makeTextField(const CppString &name, const CppString &value);

  /** Creates a html sequence for a text area (one or more lines).
    * @param name   the field name
    * @param value  the initial text
    * @param cols   the number of colums the field shall have (-1 = let browser decide)
    * @param rows   the number of rows the field shall have (-1 = let browser decide)
    * @return  the html sequence
    */
    static CppString makeTextArea(const CppString &name, const CppString &value,
                                  int cols = -1, int rows = -1);

  /** Return data which opens a select element.
    * @param name   the field name
    * @param size   number of elements that are visible at the same time.
    *               1 results usually in a listbox
    * @param multi  true: add attribute "multiple"
    * @return  the html sequence
    */
    static CppString openSelect(const CppString &name,
                                int size = 1,
                                bool multi = false);

  /** Creates an data entry for a select element.
    * @param data       the visible text for this option
    * @param value      the value which is returned with the form data.
        *               Only needed if you don't want to return the option data.
    * @param selected   the option is pre-selected in the list element.
    * @return  the html sequence
    */
    static CppString makeOption(const CppString &data,
                                const CppString &value = ULXR_PCHAR(""),
                                bool selected = false);

  /** Closes the select element.
    */
    static CppString closeSelect();

  /** Creates a html sequence for a radio button.
    * @param name     the radio name
    * @param value    the radio value
    * @param checked  true: radio is checked
    * @return  the html sequence
    */
    static CppString makeRadioButton(const CppString &name, const CppString &value,
                                     bool checked = false);

  /** Creates a html sequence for a submit button.
    * @param name     the submit button name
    * @param value    the submit button value (the visible text)
    * @return  the html sequence
    */
    static CppString makeSubmitButton(const CppString &name,
                                      const CppString &value);

  /** Creates a html sequence for a reset button.
    * @param name     the reset button name
    * @param value    the reset button value (the visible text)
    * @return  the html sequence
    */
    static CppString makeResetButton(const CppString &name,
                                     const CppString &value);

  /** Inserts a line break tag.
    */
    static CppString makeLineBreak();

  /** Surrounds a piece of text with a start and end tag.
    * The brackets < > are inserted.
    * @param data     the piece of text to be surrounded
    * @param tag      the tag name. Additional attributes for the start tag
    *                 are allowed and removed for the ending tag.
    * @param newline  true: append a newline for ease of reading
    * @return  the html sequence
    */
    static CppString applyTags(const CppString &data,
                               const CppString &tag,
                               bool newline = false);

  /** The default handler.
    * The master and subresources are searched and their handlers
    * are invoked after ectracting optional post data.
    * @param  prot       pointer to the connecthion object
    * @param  method     the method (GET, PUT ..)
    * @param  resource   name of the resource
    * @param  conn_data  already received chunk from connection
    * @return false: no resource found to handle
    */
    virtual bool handler(HttpProtocol *prot,
                         const CppString &method,
                         const CppString &resource,
                         const Cpp8BitString &conn_data);

  /** Gets the default html header.
    * The header contains the html dtd sequence, the link to the css file, sets the
    * charset to utf8, includes the title and the opening \c body tag.
    * @param title the html title
    * @return the header sequence
    */
    virtual CppString getHeader(const CppString &title) const;

  /** Gets the default html tail.
    * The default tail includes the closing \c body and \c html tags.
    * @return the tail sequence
    */
    virtual CppString getTail() const;

  /** Gets the default name for the css file.
    * By using a sngle css file you achive a common look for all
    * the pages from this handler.
    * @return the name of the css file.
    */
    virtual CppString getCssName() const;

  /** Gets the prefered encoding for this handler.
    * When in Unicode mode, UTF-8 is returned by default, otherwise we
    * use ISO-8859-1 which should work in most cases on the world.
    * @return the encoding name.
    */
    virtual CppString getEncoding() const;

  /** Returns the common css file content.
    * @param  formdata  optional data from a form
    * @param  mimetype  [OUT] \b returns the mimetype of the data
    * @return content of css file.
    */
    virtual CppString handle_css_file(const HtmlFormData &formdata, CppString &mimetype);

  /** Returns html page with a list of the subresources.
    * @param  formdata  optional data from a form
    * @param  mimetype  [OUT] \b returns the mimetype of the data
    * @return list of subresources.
    */
    CppString handle_subresource_list(const HtmlFormData &formdata, CppString &mimetype);

  /** Returns the name of the master resource.
    * @return name of the master resource.
    */
    CppString getMasterResource() const;

  /** Adds another sub-resource below the master reource.
    * @param name   name of the subresource
    * @param obj    pointer of the object which handles it
    * @param pmf    pointer of a member function in the object
    * @param descr  human readable description for this resource
    */
    template <class T>
    void addSubResource(const CppString &name,
                        T* obj,
                        CppString (T::*pmf)(const HtmlFormData &, CppString &),
                        const CppString &descr = ULXR_PCHAR(""))
    {
      for (unsigned i = 0; i < subResources.size(); ++i)
      {
        if (subResources[i]->getName() == name)
          throw RuntimeException(ApplicationError,
                                 ULXR_PCHAR("Attempt to register two resources under the same name: ")
                                 + name);
      }
      subResources.push_back(new hidden::SubResource<T>(name, obj, pmf, descr));
    }

  private:

    CppString                              masterResource;
    std::vector<hidden::SubResourceBase*>  subResources;

 private:

   // forbid them all due to internal pointers
   HtmlFormHandler(const HtmlFormHandler&);
   HtmlFormHandler& operator=(const HtmlFormHandler&);
};


}  // namespace ulxr


#endif

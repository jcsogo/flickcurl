/* -*- Mode: c; c-basic-offset: 2 -*-
 *
 * photosets-comments-api.c - Flickr flickr.photosets.comments.* API calls
 *
 * Copyright (C) 2007-2012, David Beckett http://www.dajobe.org/
 * 
 * This file is licensed under the following three licenses as alternatives:
 *   1. GNU Lesser General Public License (LGPL) V2.1 or any newer version
 *   2. GNU General Public License (GPL) V2 or any newer version
 *   3. Apache License, V2.0 or any newer version
 * 
 * You may not use this file except in compliance with at least one of
 * the above three licenses.
 * 
 * See LICENSE.html or LICENSE.txt at the top of this package for the
 * complete terms and further detail along with the license texts for
 * the licenses in COPYING.LIB, COPYING and LICENSE-2.0.txt respectively.
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef WIN32
#include <win32_flickcurl_config.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#undef HAVE_STDLIB_H
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <flickcurl.h>
#include <flickcurl_internal.h>


/**
 * flickcurl_photosets_comments_addComment:
 * @fc: flickcurl context
 * @photoset_id: The id of the photoset to add a comment to.
 * @comment_text: Text of the comment
 * 
 * Add a comment to a photoset.
 *
 * Implements flickr.photosets.comments.addComment (0.10)
 * 
 * Return value: new comment ID or non-NULL on failure
 **/
char*
flickcurl_photosets_comments_addComment(flickcurl* fc,
                                        const char* photoset_id,
                                        const char* comment_text)
{
  xmlDocPtr doc = NULL;
  xmlXPathContextPtr xpathCtx = NULL; 
  char* id = NULL;
  
  flickcurl_init_params(fc);

  if(!photoset_id || !comment_text)
    return NULL;

  flickcurl_add_param(fc, "photoset_id", photoset_id);
  flickcurl_add_param(fc, "comment_text", comment_text);

  flickcurl_end_params(fc);

  if(flickcurl_prepare(fc, "flickr.photosets.comments.addComment"))
    goto tidy;

  flickcurl_set_write(fc, 1);
  flickcurl_set_data(fc, (void*)"", 0);

  doc = flickcurl_invoke(fc);
  if(!doc)
    goto tidy;


  xpathCtx = xmlXPathNewContext(doc);
  if(!xpathCtx) {
    flickcurl_error(fc, "Failed to create XPath context for document");
    fc->failed = 1;
    goto tidy;
  }

  id = flickcurl_xpath_eval(fc, xpathCtx, (const xmlChar*)"/rsp/comment/@id");

  tidy:
  if(xpathCtx)
    xmlXPathFreeContext(xpathCtx);

  if(fc->failed)
    id = NULL;

  return id;
}


/**
 * flickcurl_photosets_comments_deleteComment:
 * @fc: flickcurl context
 * @comment_id: The id of the comment to delete from a photoset.
 * 
 * Delete a photoset comment as the currently authenticated user.
 *
 * Implements flickr.photosets.comments.deleteComment (0.10)
 * 
 * Return value: non-0 on failure
 **/
int
flickcurl_photosets_comments_deleteComment(flickcurl* fc,
                                           const char* comment_id)
{
  xmlDocPtr doc = NULL;
  int result = 1;
  
  flickcurl_init_params(fc);

  if(!comment_id)
    return 1;

  flickcurl_add_param(fc, "comment_id", comment_id);

  flickcurl_end_params(fc);

  if(flickcurl_prepare(fc, "flickr.photosets.comments.deleteComment"))
    goto tidy;

  flickcurl_set_write(fc, 1);
  flickcurl_set_data(fc, (void*)"", 0);

  doc = flickcurl_invoke(fc);
  if(!doc)
    goto tidy;

  result = 0;

  tidy:
  if(fc->failed)
    result = 1;

  return result;
}


/**
 * flickcurl_photosets_comments_editComment:
 * @fc: flickcurl context
 * @comment_id: The id of the comment to edit.
 * @comment_text: Update the comment to this text.
 * 
 * Edit the text of a comment as the currently authenticated user.
 *
 * Implements flickr.photosets.comments.editComment (0.10)
 * 
 * Return value: non-0 on failure
 **/
int
flickcurl_photosets_comments_editComment(flickcurl* fc,
                                         const char* comment_id,
                                         const char* comment_text)
{
  xmlDocPtr doc = NULL;
  int result = 1;
  
  flickcurl_init_params(fc);

  if(!comment_id || !comment_text)
    return 1;

  flickcurl_add_param(fc, "comment_id", comment_id);
  flickcurl_add_param(fc, "comment_text", comment_text);

  flickcurl_end_params(fc);

  if(flickcurl_prepare(fc, "flickr.photosets.comments.editComment"))
    goto tidy;

  flickcurl_set_write(fc, 1);
  flickcurl_set_data(fc, (void*)"", 0);

  doc = flickcurl_invoke(fc);
  if(!doc)
    goto tidy;

  result = 0;

  tidy:
  if(fc->failed)
    result = 1;

  return result;
}


/**
 * flickcurl_photosets_comments_getList:
 * @fc: flickcurl context
 * @photoset_id: The id of the photoset to fetch comments for.
 * 
 * Returns the comments for a photoset.
 *
 * Implements flickr.photosets.comments.getList (0.10)
 * 
 * Return value: array of comments or NULL on failure
 **/
flickcurl_comment**
flickcurl_photosets_comments_getList(flickcurl* fc, const char* photoset_id)
{
  xmlDocPtr doc = NULL;
  xmlXPathContextPtr xpathCtx = NULL; 
  flickcurl_comment** comments = NULL;
  int comments_count = 0;
  
  flickcurl_init_params(fc);

  if(!photoset_id)
    return NULL;

  flickcurl_add_param(fc, "photoset_id", photoset_id);

  flickcurl_end_params(fc);

  if(flickcurl_prepare(fc, "flickr.photosets.comments.getList"))
    goto tidy;

  doc = flickcurl_invoke(fc);
  if(!doc)
    goto tidy;


  xpathCtx = xmlXPathNewContext(doc);
  if(!xpathCtx) {
    flickcurl_error(fc, "Failed to create XPath context for document");
    fc->failed = 1;
    goto tidy;
  }

  comments = flickcurl_build_comments(fc, xpathCtx, 
                                    (xmlChar*)"/rsp/comments/comment", 
                                    &comments_count);


  tidy:
  if(xpathCtx)
    xmlXPathFreeContext(xpathCtx);

  if(fc->failed)
    comments = NULL;

  return comments;
}

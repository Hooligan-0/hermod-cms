/*
 * hermod-cms - An Hermod based Content Management System
 *
 * Copyright (c) 2016 Cowlab
 *
 * Hermod is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License 
 * version 3 as published by the Free Software Foundation. You
 * should have received a copy of the GNU Lesser General Public
 * License along with this program, see LICENSE file for more details.
 * This program is distributed WITHOUT ANY WARRANTY see README file.
 *
 * Authors: Saint-Genest Gwenael <gwen@hooligan0.net>
 */
#include <iostream>
#include <string>
#include <Config.hpp>
#include <ContentHtml/Template.hpp>
#include <ContentJson.hpp>
#include <ContentJson/JsonObject.hpp>
#include <ContentJson/JsonString.hpp>
#include <Database/DB.hpp>
#include <Request.hpp>
#include <String.hpp>
#include "PageNewArticle.hpp"
#include "Article.hpp"

namespace hermod {
	namespace Cms {

/**
 * @brief Constructor of the page
 *
 */
PageNewArticle::PageNewArticle(void)
    : Page()
{
	mDatabaseModule = 0;
	useSession(true);
}

/**
 * @brief This method is called when a request for this page is received
 *
 */
int PageNewArticle::process(void)
{
	if (request()->getMethod() == 1)
		showPage();
	else if (request()->getMethod() == 2)
		formProcess();
	else
	{
		// Unknown method ... :(
	}
	
	return(0);
}

void PageNewArticle::setDbModule(Module *module)
{
	mDatabaseModule = (Database::ModDatabase *)module;
}

void PageNewArticle::formProcess(void)
{
	hermod::Database::DB *db = 0;
	if (mDatabaseModule)
		db = mDatabaseModule->get("main");
	Article newArticle(db);
	
	bool isJs = false;
	try {
		// Get the form "title" element
		String title = request()->getFormValue("title");
		title.urlDecode();
		newArticle.setTitle(title);
		// Get the form "chapeau" element
		String chapeau = request()->getFormValue("chapeau");
		chapeau.urlDecode();
		newArticle.setChapeau(chapeau);
		// Get the form "content" element
		String content = request()->getFormValue("content");
		content.urlDecode();
		newArticle.setContent(content);
		// Save the new article
		newArticle.save();
		
		String Js = request()->getFormValue("mode");
		if ( ! Js.isEmpty())
		{
			isJs = true;
		}
	} catch(...) {
		std::cerr << "PageNewArticle::formProcess() Exception"<< std::endl;
	}
	
	if ( isJs )
	{
		ContentJson *content = initContentJson();
		contentJson::JsonObject *obj = new contentJson::JsonObject;
		content->setRoot(obj);
		
		obj->add("status", "ok");
		obj->add("id", String::number(newArticle.getId()) );
	}
	else
	{
		response()->header()->setRetCode(302, "Found");
		response()->header()->addHeader("Location", "/");
	}
}

void PageNewArticle::showPage(void)
{
	Config *cfg;
	cfg = Config::getInstance();

	std::string basePath = cfg->get("mod:cms", "root");
	basePath += '/';
	
	std::string page(basePath);
	page += "article_new.html";
	
	try {
		session()->setKey("last_page", "article_new");
	} catch (...) {
	}
	
	// Use the Page layer to init this document as HTML page
	ContentHtml *content = initContentHtml();
	// Load the page template
	contentHtml::Template *tpl = new contentHtml::Template;
	tpl->loadFile(page);
	// Set template as response content
	content->setTemplate( tpl );
}

	} // namespace Cms
} // namespace hermod
/* EOF */

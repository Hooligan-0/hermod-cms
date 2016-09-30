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
#include <string>
#include <Config.hpp>
#include <ContentHtml/HtmlHtml.hpp>
#include <ContentHtml/HtmlH.hpp>
#include <ContentHtml/HtmlTag.hpp>
#include <ContentHtml/Template.hpp>
#include <Database/DB.hpp>
#include <Log.hpp>
#include "PageArticle.hpp"
#include "Article.hpp"

namespace hermod {
	namespace Cms {

/**
 * @brief Constructor of the page
 *
 */
PageArticle::PageArticle(void)
    : Page()
{
	mDatabaseModule = 0;
}

/**
 * @brief This method is called when a request for this page is received
 *
 */
int PageArticle::process(void)
{
	hermod::Database::DB *db = 0;
	
	Article a;
	
	try {
		if ( ! mDatabaseModule)
			throw runtime_error("No DB defined");
		db = mDatabaseModule->get("main");
		if ( ! db)
			throw runtime_error("Main DB not configured");

		a.setDB(db);
	} catch (const odb::exception& e) {
		Log::error() << "CMS: Failed to load article : " << e.what() << Log::endl;
		return -1;
	}

	try {
		String arg = getArg(1);
		if (arg.toInt() > 0)
		{
			a.findById(arg);
		}
		else
		{
			//a.findLast();
		}
	} catch (const odb::exception& e) {
		Log::error() << "CMS: Failed to load article : " << e.what() << Log::endl;
		return -1;
	}

	try {
		Config *cfg = Config::getInstance();
		std::string basePath = cfg->get("mod:cms", "root");
		basePath += '/';

		std::string page(basePath);
		page += "article_show.html";
	
		// Use the Page layer to init this document as HTML page
		ContentHtml *content = initContentHtml();
		// Load the page template
		contentHtml::Template *tpl = new contentHtml::Template;
		tpl->loadFile(page);
		tpl->block("body").set("title",   a.title());
		tpl->block("body").set("chapeau", a.chapeau());
		tpl->block("body").set("content", a.content());
		// Set template as response content
		content->setTemplate( tpl );
	} catch (std::exception &e) {
		std::cerr << " ERR: " << e.what() << std::endl;
	}
	
	return(0);
}

void PageArticle::setDbModule(Module *module)
{
	mDatabaseModule = (Database::ModDatabase *)module;
}

	} // namespace Cms
} // namespace hermod
/* EOF */

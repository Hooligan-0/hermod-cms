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
#include <Database/DB.hpp>
#include "PageIndex.hpp"
#include "Article.hpp"

namespace hermod {
	namespace Cms {

/**
 * @brief Constructor of the page
 *
 */
PageIndex::PageIndex(void)
    : Page()
{
	mDatabaseModule = 0;
	mDbMain = 0;
	useSession(true);
}

/**
 * @brief This method is called when a request for this page is received
 *
 */
int PageIndex::process(void)
{
	if (mDbMain == 0)
		throw runtime_error("No database");
	
	Config *cfg;
	cfg = Config::getInstance();

	std::string basePath = cfg->get("mod:cms", "root");
	basePath += '/';
	
	std::string indexPage(basePath);
	indexPage += "index.html";
	
	try {
		session()->setKey("last_page", "index");
	} catch (...) {
	}
	
	Article a(mDbMain);
	try {
		a.findLast();
	} catch (...) {
	}
	
	// Use the Page layer to init this document as HTML page
	ContentHtml *content = initContentHtml();
	
	contentHtml::Template *tpl = new contentHtml::Template;
	tpl->loadFile(indexPage);
	tpl->set("first_id",      String::number( a.getId() ) );
	tpl->set("first_title",   a.title());
	tpl->set("first_chapeau", a.chapeau());
	tpl->set("first_content", a.content());
	
	content->setTemplate( tpl );
	
	return(0);
}

void PageIndex::setDbMain(Database::DB *db)
{
	mDbMain = db;
}

void PageIndex::setDbModule(Module *module)
{
	mDatabaseModule = (Database::ModDatabase *)module;
}

	} // namespace Cms
} // namespace hermod
/* EOF */

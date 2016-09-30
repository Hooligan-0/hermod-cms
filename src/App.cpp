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
#include <Module.hpp>
#include <ModuleCache.hpp>
#include <Log.hpp>
#include <Page.hpp>
#include <Database/modDatabase.hpp>
#include "App.hpp"
#include "PageIndex.hpp"
#include "PageInstall.hpp"
#include "PageArticle.hpp"
#include "PageNewArticle.hpp"

namespace hermod {

extern "C" Module* create_object()
{
	hermod::Cms::App *module;
	module = new hermod::Cms::App;
	
	return module;
}

extern "C" void destroy_object(Module *module)
{
	delete module;
}

namespace Cms {

App::App()
  : Module()
{
	// Set a name for this module
	setName("AppCms");
}

void App::freePage(Page *page)
{
	if (page == NULL)
		return;
	delete page;
}

void App::initRouter(Router *router)
{
	RouteTarget *tgt;
	// Create a target for the index page
	tgt = router->createTarget(this);
	tgt->setName("index");
	tgt->enable();
	// Create a target for the new article page
	tgt = router->createTarget(this);
	tgt->setName("article");
	tgt->enable();
	// Create a target for the new article page
	tgt = router->createTarget(this);
	tgt->setName("newArticle");
	tgt->enable();
	// Create a target for the install page
	tgt = router->createTarget(this);
	tgt->setName("install");
	tgt->enable();
}

Page *App::newPage(const std::string &name)
{
	Page   *page  = NULL;
	Database::ModDatabase *dbMod  = 0;
	Database::DB          *dbMain = 0;
	
	try {
		dbMod  = (Database::ModDatabase *)getCache()->find("Database");
		dbMain = dbMod->get("main");
	} catch (...) {
		// ToDo : log error ?
		Log::error() << "CMS: Page found but database init failed !" << Log::endl;
	}
	
	if (name.compare("index") == 0)
	{
		PageIndex *pageIndex = new PageIndex();
		pageIndex->setDbModule(dbMod);
		pageIndex->setDbMain(dbMain);
		page = pageIndex;
	}
	if (name.compare("article") == 0)
	{
		PageArticle *pageA = new PageArticle();
		pageA->setDbModule(dbMod);
		page = pageA;
	}
	if (name.compare("newArticle") == 0)
	{
		PageNewArticle *pageNA = new PageNewArticle();
		pageNA->setDbModule(dbMod);
		page = pageNA;
	}
	if (name.compare("install") == 0)
	{
		PageInstall *pageInstall = new PageInstall();
		pageInstall->setDbModule(dbMod);
		page = pageInstall;
	}
	
	return page;
}

	} // namespace Cms
} // namespace hermod
/* EOF */

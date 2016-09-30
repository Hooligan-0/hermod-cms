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
#include <odb/transaction.hxx>
#include <ContentHtml/HtmlHtml.hpp>
#include <ContentHtml/HtmlTag.hpp>
#include <ContentHtml/Template.hpp>
#include <Database/DB.hpp>
#include "PageInstall.hpp"
#include "Article.hpp"

using namespace odb::core;

namespace hermod {
	namespace Cms {

/**
 * @brief Constructor of the page
 *
 */
PageInstall::PageInstall(void)
    : Page()
{
	mDatabaseModule = 0;
}

/**
 * @brief This method is called when a request for this page is received
 *
 */
int PageInstall::process(void)
{
	hermod::Database::DB *db = 0;
	
	if (mDatabaseModule)
		db = mDatabaseModule->get("main");
	if (db)
	{
		std::cerr << "PageInstall::process" << std::endl;
		try {
			odb::core::database *dbOdb = db->get();
			transaction t (dbOdb->begin ());
			dbOdb->execute ("DROP TABLE IF EXISTS \"article\" CASCADE;");
			dbOdb->execute ("CREATE TABLE article ("
			    "\"id\" BIGINT NOT NULL PRIMARY KEY,"
			    "\"username\" INTEGER NOT NULL,"
			    "\"created\"  INTEGER NOT NULL,"
			    "\"publish\"  INTEGER NOT NULL,"
			    "\"title\" TEXT NOT NULL,"
			    "\"chapeau\" TEXT,"
			    "\"content\" TEXT NOT NULL);");
			dbOdb->execute ("DROP SEQUENCE IF EXISTS \"article_id_seq\";");
			dbOdb->execute ("CREATE SEQUENCE article_id_seq START 1;");
			t.commit ();
		} catch (const odb::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}
	
	// Use the Page layer to init this document as HTML page
	ContentHtml *content = initContentHtml();
	contentHtml::HtmlTag *tag = new contentHtml::HtmlTag("pre");
	content->root()->add( tag );
	
	return(0);
}

void PageInstall::setDbModule(Module *module)
{
	mDatabaseModule = (Database::ModDatabase *)module;
}

	} // namespace Cms
} // namespace hermod
/* EOF */

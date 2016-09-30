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
#ifndef PAGEINDEX_HPP
#define PAGEINDEX_HPP
#include <Module.hpp>
#include <Page.hpp>
#include <Database/modDatabase.hpp>

namespace hermod {
	namespace Cms {

class PageIndex: public Page
{
public:
	PageIndex();
	int process();
	void setDbModule(Module *module);
	void setDbMain  (Database::DB *db);
private:
	Database::ModDatabase *mDatabaseModule;
	Database::DB          *mDbMain;
};

	} // namespace Cms
} // namespace hermod
#endif

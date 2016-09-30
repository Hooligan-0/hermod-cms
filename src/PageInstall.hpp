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
#ifndef PAGEINSTALL_HPP
#define PAGEINSTALL_HPP
#include <Module.hpp>
#include <Page.hpp>
#include <Database/modDatabase.hpp>

namespace hermod {
	namespace Cms {

class PageInstall: public Page
{
public:
	PageInstall();
	int process();
	void setDbModule(Module *module);
private:
	Database::ModDatabase *mDatabaseModule;
};

	} // namespace Cms
} // namespace hermod
#endif

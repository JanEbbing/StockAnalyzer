/****************************************************************************
 * Copyright (C) 2014, <Copyright Holder>                                   *
 * All rights reserved                                                      *
 *                                                                          *
 * For the licensing terms see LICENSE file inside the root directory.      *
 * For the list of contributors see AUTHORS file inside the root directory. *
 ***************************************************************************/
#ifndef SRC_APP_H_
#define SRC_APP_H_

#include <iostream>

using namespace std;

/** @brief Utility class for reading application details
 *
 * This class provides some utility functions to read application details
 * such as project name, version number etc. It basically reads all the
 * specified application details via CMakeLists.txt which are autogenerated
 * by CMake and put into @ref appinfo.h */
class App {

public:
    /** @brief Returns project name
     *
     * Returns the name of the project. May contain spaces, upper/lower 
     * letters and numbers
     * @return project name */
    string getProjectName() const;
    /** @brief Returns project code name
     *
     * Returns the code name of the project. Should be all lowercase 
     * without any spaces in name
     * @return project code name */
    string getProjectCodeName() const;
    /** @brief Returns project vendor id
     *
     * Returns the vendor id of the project. Contains dot connected 
     * lowercase words, usually the reverse company url such as
     * com.example for example
     * @return project vendor id */
    string getProjectVendorID() const;
    /** @brief Returns project vendor name
     *
     * Returns the vendor name of the project. May contain spaces
     * upper/lower letters or numbers. Usually the company name is
     * taken here
     * @return project vendor name */
    string getProjectVendorName() const;
    /** @brief Returns project id
     *
     * Returns the project id. Contains dot connected lowercase words,
     * usually the company url such as example.com for example 
     * @return project id */
    string getProjectID() const;
    /** @brief Returns project major version 
     *
     * Returns the projects major version. This value is usually
     * incremented for every major release 
     * @return project major version */
    int getProjectMajorVersion() const;
    /** @brief Returns project minor version
     *
     * Returns the projects minor version. This value is usually
     * incremented for every minor release 
     * @return project minor version */
    int getProjectMinorVersion() const;
    /** @brief Returns project patch version
     *
     * Returns the projects patch version. This value is usually
     * incremented for every released patch version
     * @return project patch version */
    int getProjectPatchVersion() const;
    /** @brief Returns project version
     *
     * Returns the project version. Contains the project major,
     * minor and patch version as well as the application version
     * type such as SNAPSHOT for example
     * @return project version */
    string getProjectVersion() const;
    /** @brief Returns project copyright years
     *
     * Returns the project copyright years. Usually the year in
     * which the last release was published 
     * @return project copyright years */
    string getProjectCopyrightYears() const;
};

#endif
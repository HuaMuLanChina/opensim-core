#ifndef OPENSIM_PATH_H_
#define OPENSIM_PATH_H_
/* -------------------------------------------------------------------------- *
 *                           OpenSim: Path.h                                  *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
 * Copyright (c) 2005-2016 Stanford University and the Authors                *
 * Author(s): Carmichael Ong                                                  *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0.         *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

#include "OpenSim/Common/Object.h"

namespace OpenSim {

//==============================================================================
//                                 OPENSIM PATH
//==============================================================================
/**
* An abstract class for handling a Path. A Path refers to a list of strings that
* represent a different level in a hierarchical structure. Each level is divided
* by designated separators (e.g., "/" or "\").
*
* This class stores the list of levels as a vector of strings. One char is used
* to denote a path separator when either reading in or writing the Path out to
* a string. A bool is stored to determine whether this Path should be resolved
* relative to the root (denoted with a leading separator char) or not.
*
* @author Carmichael Ong
*/
class OSIMCOMMON_API Path : public Object {
    OpenSim_DECLARE_ABSTRACT_OBJECT(Path, Object);

public:
    /// Construct Path from a string, given separator character and a string
    /// of invalidChars. Performs a cleanPath() at the end of construction.
    Path(const std::string path,
         const char separator,
         const std::string invalidChars);

    /// Construct Path from a vector of strings (pathVec), given separator
    /// character and a string of invalidChars. Performs a cleanPath() at 
    /// the end of construction.
    Path(std::vector<std::string> pathVec,
         const char separator,
         const std::string invalidChars,
         bool isAbsolute);

    /// Use default copy constructor and assignment operator.
    Path(const Path&) = default;
    Path& operator=(const Path&) = default;

    /// Destructor.
    ~Path() = default;

    /// Write out the path to a string with each element separated by the
    /// specified separator.
    std::string getString() const;

    /// Insert a pathElement at the specified position. Note that this could
    /// cause a path to become illegal (e.g., adding ".." to the front of 
    /// an absolute path). The pathElement is checked to ensure no illegal
    /// characters are used.
    void insertPathElement(size_t pos, const std::string pathElement);

    /// Erase a pathElement at the specified position. Note that this could
    /// cause a path to become illegal (e.g., this may leave a ".." at the
    /// front of an absolute path).
    void erasePathElement(size_t pos);

    /// Append a pathElement to the Path, first checking if the pathElement
    /// is legal.
    void appendPathElement(const std::string pathElement);

    /// Cleans up a path. This includes removing "." and resolving ".." if
    /// possible (i.e. it will not remove leading ".." but otherwise will
    /// remove the previous pathElement from _path. This method also checks
    /// if a path is invalid due to an absolute path starting with "..".
    void cleanPath();

    bool isAbsolute() { return _isAbsolute; };

    size_t getPathLength() const { return _path.size(); };

protected:
    /// Default constructor
    Path() = default;

    /// Get an absolute path by resolving it relative to a given otherPath.
    /// If the current Path is already absolute, return the same Path.
    std::vector<std::string> getAbsolutePathVec(Path* otherPath);

    /// Find the relative Path between this Path and another Path (otherPath)
    /// (i.e. the Path to go FROM otherPath TO this Path). Both Paths must be 
    /// absolute.
    std::vector<std::string> getRelativePathVec(Path* otherPath);

    /// Return the sub-path, on the range [first, last).
    std::vector<std::string> getSubPathVec(size_t first, size_t last)
    {
        std::vector<std::string> subPath(_path.begin() + first, 
            _path.begin() + last);
        return subPath;
    }

    /// Return the the sub-path that contains all pathElements except for
    /// the last one.
    std::vector<std::string> getParentPathVec()
    {
        return getSubPathVec(0, getPathLength() - 1);
    }

    std::string getPathElement(size_t pos) { return _path[pos]; };
    std::string getPathName() { return _path[getPathLength() - 1]; };

private:
    // Verify that a pathElement does not contain any chars from the list
    // of _invalidChars
    bool isLegalPathElement(const std::string pathElement);

    // Path variables
    std::vector<std::string> _path;
    char _separator;
    std::string _invalidChars;
    bool _isAbsolute;
}; // end class Path

} // end of namespace OpenSim

#endif // OPENSIM_PATH_H_

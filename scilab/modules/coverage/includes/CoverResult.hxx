/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __COVER_RESULT_HXX__
#define __COVER_RESULT_HXX__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

#include "allexp.hxx"
#include "allvar.hxx"
#include "alltypes.hxx"
#include "parser.hxx"

#include "Counter.hxx"
#include "CoverMacroInfo.hxx"
#include "CodePrinterVisitor.hxx"

namespace coverage
{

class CoverResult
{

    struct __LocHelper
    {
        struct Hash
        {
            inline std::size_t operator()(const Location & l) const
            {
                return ((uint64_t)l.first_line << 48) | ((uint64_t)l.first_column << 32) | ((uint64_t)l.last_line << 16) | (uint64_t)l.last_column;
            }
        };

        struct Eq
        {
            inline bool operator()(const Location & l, const Location & r) const
            {
                return l == r;
            }
        };

        struct Compare
        {
            inline bool operator()(const Location & l, const Location & r) const
            {
                return l < r;
            }
        };

    };

    friend class CoverModule;

    std::string name;
    CoverMacroInfo info;
    uint64_t uncoveredInstrs;
    uint64_t uncoveredBranches;
    uint64_t counter;
    uint64_t time;
    std::set<Location, __LocHelper::Compare> unused;
    std::unordered_map<Location, std::vector<uint64_t>, __LocHelper::Hash, __LocHelper::Eq> branches;
    std::unordered_map<Location, uint64_t, __LocHelper::Hash, __LocHelper::Eq> loops;
    std::unordered_map<Location, uint64_t, __LocHelper::Hash, __LocHelper::Eq> times;

public:

    CoverResult(const std::string & _name, const CoverMacroInfo & _info) : name(_name), info(_info), uncoveredInstrs(0), uncoveredBranches(0), counter(0), time(0) { }

    void populate(const std::vector<Counter>::const_iterator pos, const std::vector<Counter>::const_iterator end);

    inline void setCounter(const uint64_t _counter)
    {
        counter = _counter;
    }

    inline uint64_t getCounter() const
    {
        return counter;
    }

    inline void setNanoTime(const uint64_t _time)
    {
        time = _time;
    }

    inline void addNanoTime(const uint64_t _time)
    {
        time += _time;
    }

    inline void subNanoTime(const uint64_t _time)
    {
        time -= _time;
    }

    inline uint64_t getNanoTime() const
    {
        return time;
    }

    inline double getMilliTime() const
    {
        return std::round((double)(getNanoTime() / 1000)) / 1000.;
    }

    inline double getTime() const
    {
        const double x = (double)time / 1e9;
        return std::round(x * 1000.) / 1000.;
    }


    inline uint64_t getNanoTime(const ast::Exp * e) const
    {
        auto i = times.find(e->getLocation());
        if (i == times.end() || i->first != e->getLocation())
        {
            return 0;
        }
        return i->second;
    }

    inline double getMilliTime(const ast::Exp * e) const
    {
        return std::round((double)(getNanoTime(e) / 1000)) / 1000.;
    }

    inline double getTime(const ast::Exp * e) const
    {
        const double x = (double)getNanoTime(e) / 1e9;
        return std::round(x * 1000.) / 1000.;
    }

    inline const CoverMacroInfo & getInfo() const
    {
        return info;
    }

    inline unsigned int getCovInstrsPercent() const
    {
        return info.instrsCount ? std::round(100. * (1. - (double)uncoveredInstrs / (double)info.instrsCount)) : 100.;
    }

    inline unsigned int getCovBranchesPercent() const
    {
        return info.branchesCount ? std::round(100. * (1. - (double)uncoveredBranches / (double)info.branchesCount)) : 100.;
    }

    inline uint64_t getUncInstrs() const
    {
        return uncoveredInstrs;
    }

    inline uint64_t getUncBranches() const
    {
        return uncoveredBranches;
    }

    inline const std::vector<uint64_t> getBranchesStats(const ast::Exp * e)
    {
        auto i = branches.find(e->getLocation());
        if (i == branches.end())
        {
            return std::vector<uint64_t>();
        }
        return i->second;
    }

    inline bool getLoopStats(const ast::Exp * e, uint64_t & val)
    {
        auto i = loops.find(e->getLocation());
        if (i == loops.end())
        {
            return false;
        }
        val = i->second;
        return true;
    }

    inline std::string getStringTime() const
    {
        return getStringTime(getNanoTime());
    }

    inline std::string getStringTime(const ast::Exp * e) const
    {
        return getStringTime(getNanoTime(e));
    }


    inline double getTimeRatio(const ast::Exp * e) const
    {
        return time ? ((double)getNanoTime(e) / (double)time) : 0.;
    }

    bool isCovered(const ast::Exp * e) const;
    void toXML(const std::string & outputDir);
    void toJSON(const std::string & outputDir);
    void toHTML(const std::string & outputDir);
    std::string getStringTime(const uint64_t time) const;
    void toBin(std::fstream & out) const;
    void merge(const CoverResult & cr);
    static CoverResult fromBin(std::fstream & in);

    friend std::ostream & operator<<(std::ostream & out, const CoverResult & cr);

private:

    inline static bool isInside(const Location & l1, const Location & l2)
    {
        return (l1.first_line <= l2.first_line) && (l2.first_line <= l1.last_line);
    }
};

} // namespace coverage

#endif // __COVER_RESULT_HXX__
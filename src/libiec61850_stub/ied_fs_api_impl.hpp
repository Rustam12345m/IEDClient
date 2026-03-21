/*
 *  Copyright 2023 Rustam Mustafin
 *
 *  This file is part of IEDClient.
 *
 *  IEDClient is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  IEDClient is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with IEDClient.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 * */

#pragma once

#include "cmd/interface/ied_fs_api.hpp"

namespace Libiec61850
{
    class ApiAdapter;

    class IED_FS_API_Impl : public Cmd::Interface::IED_FS_API
    {
    public:
        IED_FS_API_Impl(ApiAdapter &api) : m_api(api) {}
        ~IED_FS_API_Impl() override = default;

        int     getFileList(Core::DirOn &dir) override;
        bool    download(const QString &filename, const QString &localPath,
                         uint32_t fileSize = 0) override;
        int     remove(const QString &filename) override;

    private:
        ApiAdapter&   m_api;
    };
};
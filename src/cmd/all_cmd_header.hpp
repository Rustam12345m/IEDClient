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

#include "basic_command.hpp"

// All known commands
#include "connect_cmd.hpp"
#include "update_lds_status_cmd.hpp"
#include "update_lns_status_cmd.hpp"
#include "update_ln_cmd.hpp"
#include "update_rcbs_cmd.hpp"
#include "update_dataset_cmd.hpp"
#include "get_filelist_cmd.hpp"
#include "download_file_cmd.hpp"
#include "remove_file_cmd.hpp"
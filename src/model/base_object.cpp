
/**
 * Copyright © 2017-2018  Max Wällstedt <>
 *
 * This file is part of SimpleSCIM.
 *
 * SimpleSCIM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SimpleSCIM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with SimpleSCIM.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Further development with groups and relations support
 * by Ola Mattsson - IT informa for Sambruk
 */

#include <map>
#include <memory>
#include <iostream>

#include "base_object.hpp"
#include "../utility/simplescim_error_string.hpp"
#include "../config_file.hpp"

std::string base_object::get_uid(bool search) const {
	if (identity.empty() && search) {

		/** Get LDAP attribute that is unique identifier */
		config_file &conf = config_file::instance();
		std::string uid_attr;
		std::string type = getSS12000type();
		if (type == "base")
			uid_attr = conf.require("User-unique-identifier");
		else {
			uid_attr = conf.require(type + "-unique-identifier");
			auto pos = uid_attr.find(',');
			if (pos != std::string::npos)
				uid_attr.erase(pos);
		}
		ss12000type = type;

		if (uid_attr.empty()) {
			return "";
		}
		std::string::size_type dotPos = uid_attr.find('.');
		if (dotPos != std::string::npos)
			uid_attr = uid_attr.substr(dotPos + 1);
		/* Get unique identifier value */
		string_vector values = get_values(uid_attr);
		if (values.empty()) {
			values = get_values(type + '.' + uid_attr);
		}
		// still empty?
		if (values.empty()) {
			std::string cn;
			const string_vector &l = get_values("cn");
			if (!l.empty())
				cn = l.at(0);
			std::cerr
					<< "base_object::get_uid "
					<< cn << " missing attribute " << uid_attr
					<< " for type [" << type << "]"
					<< std::endl;
			return "";
		}

		if (values.size() != 1) {
			simplescim_error_string_set_prefix("simplescim_user_get_uid");
			simplescim_error_string_set_message("attribute \"%s\" must have exactly one value", uid_attr.c_str());
			return "";
		}

		/* Allocate unique identifier copy */
		identity = values.at(0);
	}
	return identity;
}

std::ostream &operator<<(std::ostream &os, const base_object &object) {
	static std::string quote("\"");
	static std::string tab("\t");

	os << "{";

	for (const auto &attributes: object.attributes) {
		for (const auto &value : attributes.second) {
			os << tab << quote << attributes.first << quote << "\" : \"" << value << quote << ",\n";

		}
	}

	os << "}";
	return os;
}

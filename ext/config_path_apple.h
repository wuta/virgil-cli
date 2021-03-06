/**
 * Copyright (C) 2016 Virgil Security Inc.
 *
 * Lead Maintainer: Virgil Security Inc. <support@virgilsecurity.com>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     (1) Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *     (2) Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *
 *     (3) Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @brief Cross platform methods for obtaining paths to configuration files.
 *
 * Copyright (C) 2013 Adam Nielsen <malvineous@shikadi.net>
 *
 * This code is placed in the public domain. You are free to use it for any
 * purpose. If you add new platform support, please contribute a patch!
 */

#ifndef CONFIG_PATH_H_APPLE
#define CONFIG_PATH_H_APPLE

#include <string>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>

/**
 * @brief Get an absolute path to a configuration file, specific to this user.
 *
 * Output is typically:
 *
 *     Apple: "Users/John/Library/Application Support/appname/appname.conf";
 *
 * @return get an absolute path, specific to this user.
 */
static inline std::string get_user_config_folder(const std::string& appname) {
    char* homeDir = getenv("HOME");
    if (!homeDir) {
        struct passwd* pwd = getpwuid(getuid());
        if (pwd) {
            homeDir = pwd->pw_dir;
        }
    }

    std::string pathLocalConfigFile(homeDir);
    return pathLocalConfigFile + "/Library/Application Support/" + appname;
}

/**
 * @brief Get an absolute path to a configuration file, for all users.
 *
 * Output is typically:
 *
 *     Apple: "/private/etc/appname/appname.conf";
 *
 * @param appname
 *   Short name of the application.  Avoid using spaces or version numbers, and
 *   use lowercase if possible.
 *
 * @return get an absolute path, for all users.
 */
static inline std::string get_all_user_config_folder(const std::string& appname) {
    return "/private/etc/" + appname;
}

#endif /* CONFIG_PATH_H_APPLE */

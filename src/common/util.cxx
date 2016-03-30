/**
 * Copyright (C) 2015 Virgil Security Inc.
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

#include <algorithm>
#include <iostream>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <vector>

#ifndef WIN32
#include <termios.h>
#include <unistd.h>
#endif

#include <json.hpp>

#include <virgil/crypto/VirgilByteArray.h>
#include <virgil/crypto/VirgilKeyPair.h>

#include <virgil/sdk/ServicesHub.h>
#include <virgil/sdk/io/Marshaller.h>
#include <virgil/sdk/models/PrivateKeyModel.h>

#include <cli/config.h>
#include <cli/version.h>
#include <cli/util.h>

using nlohmann::json;

namespace vsdk = virgil::sdk;
namespace vcrypto = virgil::crypto;

typedef std::pair<std::string, std::string> PairStringString;

static void setStdinEcho(bool enable) {
#ifdef WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    if (!enable) {
        mode &= ~ENABLE_ECHO_INPUT;
    } else {
        mode |= ENABLE_ECHO_INPUT;
    }
    SetConsoleMode(hStdin, mode);

#else
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if (!enable) {
        tty.c_lflag &= ~ECHO;
    } else {
        tty.c_lflag |= ECHO;
    }

    (void)tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}

std::string virgil::cli::inputShadow() {
    setStdinEcho(false);
    std::string str;
    std::cin >> std::ws;
    std::cin >> str;
    setStdinEcho(true);
    return str;
}

vcrypto::VirgilByteArray virgil::cli::setPrivateKeyPass(const vcrypto::VirgilByteArray& privateKey) {
    if (vcrypto::VirgilKeyPair::isPrivateKeyEncrypted(privateKey)) {
        std::string privateKeyPass;
        std::cout << "Enter private key password:" << std::endl;
        privateKeyPass = inputShadow();
        vcrypto::VirgilByteArray privateKeyPassByteArray = vcrypto::str2bytes(privateKeyPass);
        if (vcrypto::VirgilKeyPair::checkPrivateKeyPassword(privateKey, privateKeyPassByteArray)) {
            return privateKeyPassByteArray;
        } else {
            throw std::runtime_error("private key pass is invalid");
        }
    }
    return vcrypto::VirgilByteArray();
}

bool virgil::cli::isPublicKeyModel(const std::string& publicKey) {
    std::istringstream iss(publicKey);
    std::string firstLine;
    std::getline(iss, firstLine);
    try {
        json tmp = json::parse(publicKey);
        return tmp.is_object() && tmp.find("id") != tmp.end() && tmp.find("public_key") != tmp.end() &&
               tmp.find("created_at") != tmp.end();
    } catch (std::exception& exception) {
        return false;
    }
}

bool virgil::cli::isPrivateKeyModel(const std::string& privateKey) {
    std::istringstream iss(privateKey);
    std::string firstLine;
    std::getline(iss, firstLine);
    try {
        json tmp = json::parse(privateKey);
        return tmp.is_object() && tmp.find("private_key") != tmp.end() && tmp.find("virgil_card_id") != tmp.end();
    } catch (std::exception& exception) {
        return false;
    }
}

void virgil::cli::printVersion(std::ostream& out, const char* programName) {
    out << programName << "  "
        << "version: " << virgil::cli_version() << std::endl;
}

//-------------------------------------------------------------------------------------

void virgil::cli::checkFormatRecipientArg(const std::pair<std::string, std::string>& pairRecipientArg) {
    const std::string type = pairRecipientArg.first;
    if (type != "password" && type != "id" && type != "vcard" && type != "email" && type != "pubkey") {
        throw std::invalid_argument("invalid type format: " + type + ". Expected format: '<key>:<value>'. "
                                                                     "Where <key> = [password|id|vcard|email|pubkey]");
    }
}

void virgil::cli::checkFormatIdentity(const std::string& args, const std::string& type) {
    if (type != "email") {
        throw std::invalid_argument(args + " invalid type format: " + type + ". Expected format: '<key>:<value>'. "
                                                                             "Where <key> = [email].");
    }
}

//-------------------------------------------------------------------------------------

vcrypto::VirgilByteArray virgil::cli::readFileBytes(const std::string& in) {
    std::ifstream inFile(in, std::ios::in | std::ios::binary);
    if (!inFile) {
        throw std::invalid_argument("can not read file: " + in);
    }
    return vcrypto::VirgilByteArray((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
}

std::string virgil::cli::readInput(const std::string& in) {
    if (in.empty() || in == "-") {
        return std::string((std::istreambuf_iterator<char>(std::cin)), std::istreambuf_iterator<char>());
    } else {
        std::ifstream inFile(in, std::ios::in | std::ios::binary);
        if (!inFile) {
            throw std::invalid_argument("can not read file: " + in);
        }
        return std::string((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    }
}

vsdk::dto::ValidatedIdentity virgil::cli::readValidateIdentity(const std::string& in) {
    std::ifstream inFile(in, std::ios::in | std::ios::binary);
    if (!inFile) {
        throw std::invalid_argument("cannot read file: " + in);
    }
    std::string validatedIdentityStr((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    return vsdk::io::Marshaller<vsdk::dto::ValidatedIdentity>::fromJson(validatedIdentityStr);
}

vsdk::models::CardModel virgil::cli::readCard(const std::string& in) {
    std::ifstream inFile(in, std::ios::in | std::ios::binary);
    if (!inFile) {
        throw std::invalid_argument("cannot read file: " + in);
    }
    std::string cardStr((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    return vsdk::io::Marshaller<vsdk::models::CardModel>::fromJson(cardStr);
}

virgil::crypto::VirgilByteArray virgil::cli::readPublicKey(const std::string& in) {
    vcrypto::VirgilByteArray publicKey;
    std::string pathToPublicKeyFile = in;
    std::ifstream inFile(pathToPublicKeyFile, std::ios::in | std::ios::binary);
    if (!inFile) {
        throw std::invalid_argument("cannot read file: " + pathToPublicKeyFile);
    }
    std::string publicKeyStr((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());

    if (virgil::cli::isPublicKeyModel(publicKeyStr)) {
        vsdk::models::PublicKeyModel publicKeyModel =
            vsdk::io::Marshaller<vsdk::models::PublicKeyModel>::fromJson(publicKeyStr);
        publicKey = publicKeyModel.getKey();
    } else {
        publicKey = vcrypto::str2bytes(publicKeyStr);
    }
    return publicKey;
}

virgil::crypto::VirgilByteArray virgil::cli::readPrivateKey(const std::string& in) {
    vcrypto::VirgilByteArray privateKey;
    std::string pathToPrivateKeyFile = in;
    std::ifstream inFile(pathToPrivateKeyFile, std::ios::in | std::ios::binary);
    if (!inFile) {
        throw std::invalid_argument("cannot read file: " + pathToPrivateKeyFile);
    }
    std::string privateKeyStr((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    if (virgil::cli::isPrivateKeyModel(privateKeyStr)) {
        vsdk::models::PrivateKeyModel privateKeyModel =
            vsdk::io::Marshaller<vsdk::models::PrivateKeyModel>::fromJson(privateKeyStr);
        privateKey = privateKeyModel.getKey();
    } else {
        privateKey = vcrypto::str2bytes(privateKeyStr);
    }
    return privateKey;
}

//-------------------------------------------------------------------------------------

void virgil::cli::writeBytes(const std::string& out, const vcrypto::VirgilByteArray& data) {
    if (out.empty()) {
        std::copy(data.begin(), data.end(), std::ostreambuf_iterator<char>(std::cout));
        std::cout << std::endl;
        return;
    }

    std::ofstream outFile(out, std::ios::out | std::ios::binary);
    if (!outFile) {
        throw std::invalid_argument("cannot write file: " + out);
    }
    std::copy(data.begin(), data.end(), std::ostreambuf_iterator<char>(outFile));
}

void virgil::cli::writeBytes(const std::string& out, const std::string& data) {
    return virgil::cli::writeBytes(out, virgil::crypto::str2bytes(data));
}

//-------------------------------------------------------------------------------------

std::string virgil::cli::getDescriptionMessage(const std::string description, std::vector<std::string> examples) {
    std::string descriptionMessage;
    descriptionMessage += "\nDESCRIPTION:\n" + description;
    if (!examples.empty()) {
        descriptionMessage += "EXAMPLES:\n";
        for (const auto& example : examples) {
            descriptionMessage += example;
        }
    }
    return descriptionMessage;
}

//-------------------------------------------------------------------------------------

std::vector<vsdk::models::CardModel> virgil::cli::getRecipientCards(const std::string& type, const std::string& value,
                                                                    const bool includeUnconrimedCard) {

    std::vector<vsdk::models::CardModel> recipientCards;
    vsdk::ServicesHub servicesHub(VIRGIL_ACCESS_TOKEN);
    if (type == "id") {
        recipientCards.push_back(servicesHub.card().get(value));
    } else if (type == "email") {
        vsdk::dto::Identity identity(value, vsdk::models::IdentityModel::Type::Email);
        std::vector<vsdk::models::CardModel> cards;
        cards = servicesHub.card().search(identity, includeUnconrimedCard);
        recipientCards.insert(std::end(recipientCards), std::begin(cards), std::end(cards));
    } else if (type == "vcard") {
        std::string pathTofile = value;
        std::ifstream inFile(pathTofile, std::ios::in | std::ios::binary);
        if (!inFile) {
            throw std::invalid_argument("cannot read file: " + pathTofile);
        }

        // in file may be card or cards
        std::string undefinedCardJsonStr((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        try {
            json tmp = json::parse(undefinedCardJsonStr);
            if (tmp.is_object()) {
                vsdk::models::CardModel card =
                    vsdk::io::Marshaller<vsdk::models::CardModel>::fromJson(undefinedCardJsonStr);
                recipientCards.push_back(card);
            } else if (tmp.is_array()) {
                std::vector<vsdk::models::CardModel> cards = vsdk::io::cardsFromJson(undefinedCardJsonStr);
                recipientCards.insert(std::end(recipientCards), std::begin(cards), std::end(cards));
            } else {
                throw std::runtime_error("Can't parse " + pathTofile + " .");
            }

        } catch (std::exception& exception) {
            throw std::runtime_error("Can't parse " + pathTofile + " .");
        }
    }

    return recipientCards;
}

std::vector<std::string> virgil::cli::getRecipientCardsId(const std::string& type, const std::string& value,
                                                          const bool includeUnconrimedCard) {
    std::vector<vsdk::models::CardModel> recipientCards =
        virgil::cli::getRecipientCards(type, value, includeUnconrimedCard);
    std::vector<std::string> recipientCardsId;
    for (const auto& recipientCard : recipientCards) {
        recipientCardsId.push_back(recipientCard.getId());
    }
    return recipientCardsId;
}

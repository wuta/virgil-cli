NAME
====

**card-create-global** -- create [a Global Virgil Card](https://github.com/VirgilSecurity/virgil/wiki/Virgil-Glossary#global-virgil-card).

SYNOPSIS
========

       virgil card-create-global  {-e <arg>|--public-key <file>} {-d <arg>|-f <file>}
                           [-o <file>] -k <file> [-p <arg>] [-V] [--]
                           [--version] [-h]

DESCRIPTION
===========

A Virgil Card is the main entity of the Keys Service, it includes the information about the user and his public key. The Virgil Card identifies the user by one of his available types, such as an email, a phone number, etc.

The Virgil Card might be *global* and *private*. The difference is whether Virgil Services take part in the Identity verification.

*Global Virgil Cards* are created with the validation token received after verification in Virgil Identity Service. Any developer with Virgil account can create a Global Virgil Card and you can be sure that the account with a particular email has been verified and the email owner is really the Identity owner.

Connections between Cards can be created, Cards can be with:

1.  one public key and *multiple* Identities;
2.  one public key and *one* Identity;
3.  multiple public keys and *multiple* Identities;
4.  multiple public keys and *one* Identity;
5.  one public key and *one* Identity connected with `public-key-id`;
6.  one public key and *multiple* Identities connected with `public-key-id`.

OPTIONS
=======

        -e <arg>,  --public-key-id <arg>
         (OR required)  Public key identifier.
             -- OR --
        --public-key <file>
         (OR required)  Public key.


        -d <arg>,  --identity <arg>
         (OR required)  Identity: email.
             -- OR --
        -f <file>,  --validated-identity <file>
         (OR required)  Validated identity
         (see 'virgil identity-confirm-global').


        -o <file>,  --out <file>
         Global Virgil Card. If omitted, stdout is used.

        -k <file>,  --key <file>
         (required)  Private key.

        -p <arg>,  --private-key-password <arg>
         Private Key Password.

        -V,  --VERBOSE
         Shows detailed information..

        --,  --ignore_rest
         Ignores the rest of the labeled arguments following this flag.

        --version
         Displays version information and exits.

        -h,  --help
         Displays usage information and exits.

EXAMPLES
========

1.  Create a [Global Virgil Card](https://github.com/VirgilSecurity/virgil/wiki/Virgil-Glossary#global-virgil-card):

        virgil card-create-global -f alice/validated_identity_global.txt --public-key public.key -k alice/private.key -o alice/my_card.vcard

2.  Create a Global Virgil Card, with Identity confirmation:

        virgil card-create-global -d alice@domain.com --public-key public.key -k alice/private.key -o alice/my_card.vcard

3.  Create a connection with already existing Global Virgil Card, by [public-key-id](https://github.com/VirgilSecurity/virgil/wiki/Virgil-Glossary#public-key-id):

        virgil card-create-global -f alice/validated_identity_global.txt -e <pub_key_id> -k alice/private.key -o alice/my_card.vcard

4.  Create a connection with already existing Global Virgil Card, by public-key-id. With confirming of identity:

        virgil card-create-global -d alice@domain.com -e <pub_key_id> -k alice/private.key -o alice/my_card.vcard

SEE ALSO
========

virgil(1)
virgil-config(1)
virgil-keygen(1)
virgil-identity-confirm-global(1)

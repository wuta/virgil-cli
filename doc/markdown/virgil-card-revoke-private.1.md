NAME
====

**card-revoke-private** -- revoke [a Private Virgil Card](https://github.com/VirgilSecurity/virgil/wiki/Virgil-Glossary#private-virgil-card) from the Keys Service.

SYNOPSIS
========

        virgil card-revoke-private  -a <arg> [-f <file>] -k <file> [-p <arg>] [-V] [--]
                            [--version] [-h]

DESCRIPTION
===========

The utility allows you to revoke a Private Virgil Card from the Keys Service.

OPTIONS
=======

        -a <arg>,  --card-id <arg>
         (required)  Private Virgil Card identifier

        -f <file>,  --validated-identity <file>
         Private Validated identity. See 'virgil identity-confirm-private'

        -k <file>,  --key <file>
         (required)  Private key

        -p <arg>,  --private-key-password <arg>
         Private Key Password.

        -V,  --VERBOSE
         Shows detailed information.

        --,  --ignore_rest
         Ignores the rest of the labeled arguments following this flag.

        --version
         Displays version information and exits.

        -h,  --help
         Displays usage information and exits.

EXAMPLES
========

1.  Revoke a Private Virgil Card with [a confirmed identity](https://github.com/VirgilSecurity/virgil/wiki/Virgil-Glossary#confirmed-identity):

        virgil card-revoke -a <card_id> -f validated-identities.txt -k private.key

2.  Revoke Virgil Card with [an unconfirmed Identity](https://github.com/VirgilSecurity/virgil/wiki/Virgil-Glossary#unconfirmed-identity):

        virgil card-revoke -a <card_id> -k private.key

SEE ALSO
========

virgil(1)
virgil-config(1)
virgil-card-create-private(1)
virgil-public-key-revoke-private(1)

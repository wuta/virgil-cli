NAME
====

**card-get** -- return Global/Private Virgil Card(s)

SYNOPSIS
========

        virgil card-get [-o <arg>] -a <arg> [-e <arg>] [-k <file>] [-p <arg>] [-V]
                     [--] [--version] [-h]

DESCRIPTION
===========

The utility allows you to get a Private/Global Virgil Card by [`card-id`](https://github.com/VirgilSecurity/virgil/wiki/Virgil-Glossary#card-id) or a group of Private/Global Cards connected with a [`public-key-id`](https://github.com/VirgilSecurity/virgil/wiki/Virgil-Glossary#public-key-id).

OPTIONS
=======

        -o <arg>,  --out <arg>
         Folder where Virgil Cards will be saved.

        -a <arg>,  --card-id <arg>
         (required)  Virgil Card identifier

        -e <arg>,  --public-key-id <arg>
         Public Key identifier


        -k <file>,  --key <file>
         Private key

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

1.  Return a Private/Global Virgil Card by card-id:

        virgil card-get -a <card_id> -o cards/

2.  Return a group of Private/Global Virgil Cards connected with public-key-id, card-id belongs to one of the Cards:

        virgil card-get -a <card_id> -e <public_key_id> -k private.key -o cards/

SEE ALSO
========

virgil(1)
virgil-config(1)
virgil-card-create-global(1)
virgil-card-create-private(1)
virgil-card-search-global(1)
virgil-card-search-private(1)

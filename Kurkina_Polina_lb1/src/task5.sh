#!/bin/bash
types='- b c d l p s'

for type in $types; do
        find ~+ / -exec bash -c "ls -l -d \"{}\" | grep \"^$type\" && stat {}" \; -quit 2>/dev/null | tee $1
done

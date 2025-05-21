#!/bin/bash
types='- b c d l p s'

for type in $types; do
        find ~+ / -exec bash -c "ls -l -d \"{}\" | grep \"^$type\" " \; -quit 2>/dev/null >> $1
done

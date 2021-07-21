#!/bin/bash

# Variables
indexGenerator="$HOME/Documentos/PROJECTS/ConditionalBinOps/SourceCodes/IndexGenerator/Debug/IndexGenerator"
condOperator="$HOME/Documentos/PROJECTS/ConditionalBinOps/SourceCodes/CondBinOps/Debug/CondBinOps"
nrTests=100
nrRepetitions=5

# Creating index structures
for i in {0..5}
do
   echo -e "CREATING INDEX FOR T1.data $i\n================================="
  $indexGenerator T1.data $i simple
done

# Remove past results
echo "REMOVING PAST RESULTS"
rm */*.*Time

# Executing all tests
for (( iTest=1; iTest<=$nrTests; iTest++ ))
do
   cd "TEST $iTest"
   # repeting the execution <nrRepetions> times
   for (( iRep=1; iRep<=$nrRepetitions; iRep++ ))
   do
	printf "\n        TEST $iTest/$iRep PERFORMING RELATIONAL CONDITIONAL FOR ALL OPERATION - INDEX \n"
	printf "==============================================================\n"
	$condOperator ../T1.data T2.data ../query.sql -groups ../TG.data -index T1

	printf "\n        TEST $iTest/$iRep PERFORMING RELATIONAL CONDITIONAL FOR ALL OPERATION - FTS \n"
	printf "==============================================================\n"
	$condOperator ../T1.data T2.data ../query.sql -groups ../TG.data
   done
   cd ..
done

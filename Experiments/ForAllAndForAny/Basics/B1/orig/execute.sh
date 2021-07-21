indexCreator="../../../SourceCodes/IndexGenerator/Debug/IndexGenerator"
condOperator="../../../SourceCodes/CondBinOps/Debug/CondBinOps"

printf "\n                  CREATING INDEX FOR T1[0]\n"
printf "==============================================================\n"
./$indexCreator T1.data 0 simple
printf "\n                  CREATING INDEX FOR T1[1]\n"
printf "==============================================================\n"
./$indexCreator T1.data 1 simple
printf "\n                  CREATING INDEX FOR T1[2]\n"
printf "==============================================================\n"
./$indexCreator T1.data 2 simple
printf "\n                  CREATING INDEX FOR T1[3]\n"
printf "==============================================================\n"
./$indexCreator T1.data 3 simple
printf "\n                  CREATING INDEX FOR T1[4]\n"
printf "==============================================================\n"
./$indexCreator T1.data 4 simple

printf "\n               PERFORMING RELATIONAL CONDITIONAL FOR ALL OPERATION - INDEX \n"
printf "==============================================================\n"
./$condOperator T1.data T2.data queryForAll.sql -groups TG.data -index T1

printf "\n               PERFORMING RELATIONAL CONDITIONAL FOR ALL OPERATION - FTS \n"
printf "==============================================================\n"
./$condOperator T1.data T2.data queryForAll.sql -groups TG.data

printf "\n               PERFORMING RELATIONAL CONDITIONAL FOR ANY OPERATION - INDEX \n"
printf "==============================================================\n"
./$condOperator T1.data T2.data queryForAny.sql -groups TG.data -index T1

printf "\n               PERFORMING RELATIONAL CONDITIONAL FOR ANY OPERATION - FTS \n"
printf "==============================================================\n"
./$condOperator T1.data T2.data queryForAny.sql -groups TG.data

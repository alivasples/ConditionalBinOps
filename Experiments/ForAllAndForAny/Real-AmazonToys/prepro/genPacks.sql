SELECT id1
FROM Pairs
GROUP BY product1
HAVING COUNT(*) > 50
INTO OUTFILE 'topProducts.csv'
LINES TERMINATED BY '\n'
;

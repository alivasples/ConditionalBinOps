-- CREATE A TABLE TO STORE PAIR OF SELLERS WITH EQUAL PRODUCTS
DROP TABLE IF EXISTS Pairs;
CREATE TABLE Pairs(
	id1 INT
	, idGroup1 FLOAT
	, seller1 VARCHAR(136)
	, category1 VARCHAR(121)
	, product1 VARCHAR(236)
	, stock1 FLOAT
	, price1 FLOAT
	, id2 INT
	, idGroup2 FLOAT
	, seller2 VARCHAR(136)
	, category2 VARCHAR(121)
	, product2 VARCHAR(236)
	, stock2 FLOAT
	, price2 FLOAT
	, INDEX(id1)
	, INDEX(id2)
);

-- Populate the table
INSERT INTO Pairs
SELECT * 
FROM T1 AS S1
INNER JOIN T1 AS S2
    ON S1.seller != S2.seller
    AND S1.product = S2.product
    AND S1.category = S2.category
;

-- CREATE A TABLE TO STORE PAIRS OF PACK SUPPLIERS
DROP TABLE IF EXISTS PackSellers;
CREATE TABLE PackSellers(
	idPack INT
	, seller1 VARCHAR(136)
	, seller2 VARCHAR(136)
	, nrCommonProds INT
	, INDEX(idPack)
	, INDEX(seller1)
	, INDEX(seller2)
);

-- Populate the table
set @counter=-1;
INSERT INTO PackSellers
SELECT @counter:=@counter+1 AS idPack, PackSellers.* 
FROM
(
    SELECT seller1, seller2, COUNT(*) nrSameProducts
    FROM Pairs
    GROUP BY seller1, seller2
    HAVING COUNT(*) > 4
) AS PackSellers
;


-- SOME QUERIES
-- Select the more repeated products
SELECT product1, COUNT(*) repeats
FROM Pairs
GROUP BY product1
ORDER BY repeats DESC
;

-- Select the pairs of sellers that have more thant 1 common products
SELECT seller1, seller2, COUNT(*) nrSameProducts
FROM Pairs
-- WHERE product1 = ''
GROUP BY seller1, seller2
HAVING nrSameProducts > 1
;

-- Choose one of the pair of sellers and execute this query to list the products they have in common
SELECT *
FROM Pairs
WHERE seller1 = "a1 Toys" and seller2 = "The Toy Center"
;

-- Grouping and counting way to perform the relational condition
-- We've created a temporary T2 - Table of Requirements
SELECT idGroup, seller, COUNT(*)
FROM T1
WHERE EXISTS
(
    SELECT product FROM
    (
        SELECT 'BrainBox - Art' AS product
        UNION SELECT 'BrainBox - World History' AS product
    ) 
    AS T2
    WHERE T1.product = T2.product -- Operation between tuples
)
GROUP BY idGroup, seller
HAVING COUNT(*) = 2 -- Need to be the size of T2 when "For All" or > 0 when "For Any"
;


-- SELECT GROUPS OF COMMON PRODUCTS FROM 2 DIFFERENT SELLERS
SELECT SE.idPack, SE.nrCommonProds, PR.id1, PR.product1, PR.seller1, PR.seller2
FROM PackSellers AS SE
INNER JOIN Pairs AS PR
    ON PR.seller1 = SE.seller1 
    AND PR.seller2 = SE.seller2
ORDER BY idPack

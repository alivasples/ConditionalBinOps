T1 ForAll T2 ON 
T1.Category = T2.Category AND 
T1.Product  = T2.Product  AND  
(( T1.Stock >= T2.Stock   AND
   T1.Price <= T2.Price)  OR
   T1.Price <= T2.Price / 2
)

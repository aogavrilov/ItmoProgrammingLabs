/* LAB3 - BD */
/* TASK 3 */
SELECT SC.Name, COUNT(*) AS COUNT
FROM 
	Production.ProductSubcategory AS C INNER JOIN Production.Product AS P ON C.ProductSubcategoryID = P.ProductSubcategoryID
	INNER JOIN Production.ProductCategory AS SC ON C.ProductCategoryID = SC.ProductCategoryID
GROUP BY SC.Name
/* TASK 4 */
SELECT SC.Name, COUNT(*) AS COUNT
FROM
	Production.ProductSubcategory AS SC INNER JOIN Production.Product AS P
	ON SC.ProductSubcategoryID = P.ProductSubcategoryID
GROUP BY SC.Name
/* TASK 5 */
SELECT TOP 3 SC.Name
FROM
	Production.ProductSubcategory AS SC INNER JOIN Production.Product AS P
	ON SC.ProductSubcategoryID = P.ProductSubcategoryID
GROUP BY SC.Name
ORDER BY COUNT(*) ASC 
/* TASK 6 */
SELECT TOP 1 SC.Name, P.ListPrice 
FROM 
	Production.Product AS P INNER JOIN
	Production.ProductSubcategory AS SC
	ON P.ProductSubcategoryID = SC.ProductSubcategoryID
WHERE P.Color = 'RED'
GROUP BY SC.Name, P.ListPrice
ORDER BY P.ListPrice DESC
/* TASK 7 */
SELECT V.Name, COUNT(*) AS AMOUNT
FROM
	Purchasing.ProductVendor AS PV INNER JOIN
	Production.Product AS P	ON PV.ProductID = P.ProductID INNER JOIN
	Purchasing.Vendor AS V ON V.BusinessEntityID = PV.BusinessEntityID
GROUP BY V.Name 
/* TASK 8 */
SELECT P.Name
FROM 
	Purchasing.ProductVendor AS PV INNER JOIN 
	Purchasing.Vendor AS V ON V.BusinessEntityID = PV.BusinessEntityID INNER JOIN
	Production.Product AS P ON P.ProductID = PV.ProductID
GROUP BY P.Name
HAVING COUNT(*) > 1
/* TASK 9 */
SELECT TOP 1 P.NAME, COUNT(*) AS COUNT
FROM 
	Production.Product AS P INNER JOIN
	Purchasing.ProductVendor AS PV ON P.ProductID = PV.ProductID INNER JOIN
	Purchasing.Vendor AS V ON PV.BusinessEntityID = V.BusinessEntityID
GROUP BY P.Name
ORDER BY COUNT(*) DESC
/* task 10 */
SELECT TOP 1 PC.NAME
FROM
	Production.Product AS P INNER JOIN 
	Production.ProductSubcategory AS PSC ON P.ProductSubcategoryID =  PSC.ProductSubcategoryID 
	INNER JOIN Production.ProductCategory AS PC ON PC.ProductCategoryID = PSC.ProductCategoryID
GROUP BY PC.Name
ORDER BY COUNT(*) DESC
/* TASK 11 */
SELECT C.Name, COUNT(DISTINCT SC.ProductSubcategoryID) AS 'SUBC AMOUNT', COUNT(*) AS AMOUNT
FROM
	Production.Product AS P INNER JOIN 
	Production.ProductSubcategory AS SC ON P.ProductSubcategoryID = SC.ProductSubcategoryID INNER JOIN
	Production.ProductCategory AS C ON SC.ProductCategoryID = C.ProductCategoryID
GROUP BY C.Name
/* TASK 12 */
SELECT V.CreditRating, COUNT(*) AS AMOUNT
FROM
	Purchasing.ProductVendor AS PC INNER JOIN 
	Purchasing.Vendor AS V ON V.BusinessEntityID = PC.BusinessEntityID INNER JOIN
	Production.Product AS P ON P.ProductID = PC.ProductID 
GROUP BY V.CreditRating
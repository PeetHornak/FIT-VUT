--Index

drop index myIndex;

explain plan set statement_id = 'analysisBefore' for
select PIVOVAR.ID, PIVOVAR.NAZOV, COUNT(PIVOVAR.ID) as pocet_zamestnancov
from PIVOVAR inner join SLADEK on PIVOVAR.ID = SLADEK.PIVOVAR_ID
group by PIVOVAR.ID, PIVOVAR.NAZOV
order by pocet_zamestnancov desc ;
select PLAN_TABLE_OUTPUT FROM TABLE(DBMS_XPLAN.display('plan_table', 'analysisBefore', 'typical'));

create index myIndex on SLADEK (PIVOVAR_ID);

explain plan set statement_id = 'analysisAfter' for
select PIVOVAR.ID, PIVOVAR.NAZOV, COUNT(PIVOVAR.ID) as pocet_zamestnancov
from PIVOVAR inner join SLADEK on PIVOVAR.ID = SLADEK.PIVOVAR_ID
group by PIVOVAR.ID, PIVOVAR.NAZOV
order by pocet_zamestnancov desc ;
select PLAN_TABLE_OUTPUT FROM TABLE(DBMS_XPLAN.display('plan_table', 'analysisAfter', 'typical'));

--View


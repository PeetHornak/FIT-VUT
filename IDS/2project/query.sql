-- Uzivatelia (login) ktori aspon raz hodnotili hospodu, zoradeni od najmladsieho (vek)
select login
from (
       select UZIVATEL.LOGIN as login, UZIVATEL.VEK
       from UZIVATEL inner join HODNOTI_HOSPODA HH on UZIVATEL.LOGIN = HH.UZIVATEL_ID
       order by UZIVATEL.VEK asc
     )
group by login;

-- Pocet hodnoteni piva od uzivatela
select COUNT(login)
from (select UZIVATEL.LOGIN as login
      from UZIVATEL inner join HODNOTI_PIVO HP on UZIVATEL.LOGIN = HP.UZIVATEL_ID
     );

-- Piva typu IPA ktore su povodom z Mnichova
select P.ID, P.NAZOV
from PIVO P
where P.TYP = 'IPA' and P.ID in (
      select P.ID
      from PIVO P inner join VYROBENEZ_SLAD VS on P.ID = VS.PIVO_ID inner join SLAD S on VS.SLAD_ID = S.ID
      where S.PUVOD = 'Mnichov'
  );

-- Sladek ktory nema pivovar
select *
from SLADEK
where NOT EXISTS(
  select *
  from PIVOVAR
  where SLADEK.PIVOVAR_ID = PIVOVAR.ID
  );

-- Pocet piv s hodnotou farby vacsou ako 50 (SRM) a ich priemerna hodnota
select COUNT(*), AVG(FARBA_SRM)
from PIVO
where FARBA_SRM > 50;

-- Priemerne hodnotenie piva od uzivatela
select UZIVATEL_ID, AVG(HODNOTENIE) as priemerne_hodnotenie
from HODNOTI_PIVO
group by UZIVATEL_ID;

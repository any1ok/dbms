select name

from Pokemon

where type = 'grass' 

order by name;



select name

from Trainer

where hometown ='Brown City' or hometown = 'Rainbow City'

order by name;



select DISTINCT type

from Pokemon

order by type;



select name

from City

where name like "B%"

order by name;



select hometown

from Trainer

where name not like "M%"

order by hometown;



select nickname

from CatchedPokemon

where CatchedPokemon.level in (select max(CatchedPokemon.level) 

                from CatchedPokemon

                )

order by CatchedPokemon.nickname;



select Pokemon.name

from Pokemon

where name like 'A%' or name like 'E%' or name like 'I%' or name like 'O%' or name like 'U%'

order by Pokemon.name;



select AVG(level)

from CatchedPokemon;



select max(level)

from CatchedPokemon,Trainer

where Trainer.id = CatchedPokemon.owner_id AND Trainer.name = 'Yellow';



select DISTINCT hometown

from Trainer

order by hometown;



select name,nickname

from Trainer,CatchedPokemon

where nickname like 'A%' and owner_id = Trainer.id

order by name;



select Trainer.name

from City,Trainer,Gym

where Trainer.id = Gym.leader_id and Gym.city = City.name and City.description = 'Amazon';



select CatchedPokemon.owner_id,count(CatchedPokemon.owner_id)

from CatchedPokemon,Pokemon

where CatchedPokemon.pid = Pokemon.id and Pokemon.type = 'Fire'

Group by CatchedPokemon.owner_id

order by count(CatchedPokemon.owner_id) DESC limit 1;



select DISTINCT Pokemon.type

from Pokemon

where Pokemon.id <= 9

order by Pokemon.id DESC;



select count(Pokemon.id)

from Pokemon

where not Pokemon.type = 'Fire'; 



select Pokemon.name

from Pokemon,Evolution

where Evolution.before_id > Evolution.after_id and Evolution.before_id = Pokemon.id

order by Pokemon.name;



select AVG(CatchedPokemon.level)

from CatchedPokemon,Pokemon

where Pokemon.type = 'Water' and Pokemon.id = CatchedPokemon.pid;



select CatchedPokemon.nickname

from CatchedPokemon,Gym

where CatchedPokemon.owner_id = Gym.leader_id

order by CatchedPokemon.level DESC limit 1 ;



select Trainer.name

from Trainer,CatchedPokemon

where Trainer.hometown = 'Blue City' and CatchedPokemon.owner_id = Trainer.id

Group by owner_id

order by AVG(CatchedPokemon.level) DESC Limit 1;



select a.pname

from (

  select Trainer.name AS Tn, count(Trainer.hometown) AS cnt,CatchedPokemon.pid AS pid,Pokemon.name AS pname

  from Trainer,CatchedPokemon,Pokemon,Evolution

  where Trainer.id = CatchedPokemon.owner_id and pid = Evolution.before_id and pid = Pokemon.id and Pokemon.type = 'Electric'

  group by Pokemon.name,Trainer.hometown

) a

where a.cnt = 1;



select Trainer.name,sum(CatchedPokemon.level)

from Trainer,Gym,CatchedPokemon

where Trainer.id = Gym.leader_id and Trainer.id = CatchedPokemon.owner_id

Group by Trainer.name

order by sum(CatchedPokemon.level) DESC;



select Trainer.hometown

from Trainer

group by Trainer.hometown

order by count(Trainer.hometown) DESC limit 1;



select DISTINCT Pokemon.name

from Pokemon,CatchedPokemon,Trainer

where Pokemon.id = CatchedPokemon.pid 

	And Pokemon.id IN(

		select CatchedPokemon.pid

		from Trainer,CatchedPokemon

		where Trainer.hometown = 'Sangnok City' and Trainer.id = CatchedPokemon.owner_id)

	And Pokemon.id IN(

		select CatchedPokemon.pid

		from Trainer,CatchedPokemon

		where Trainer.hometown = 'Brown City' and Trainer.id = CatchedPokemon.owner_id)

order by Pokemon.name;



select Trainer.name

from Trainer,Pokemon,CatchedPokemon

where Trainer.id = CatchedPokemon.owner_id and CatchedPokemon.pid = Pokemon.id and Trainer.hometown = 'Sangnok City' and Pokemon.name like 'P%'

order by Trainer.name;



select Trainer.name,Pokemon.name

from Trainer,Pokemon,CatchedPokemon

where Trainer.id = CatchedPokemon.owner_id and CatchedPokemon.pid = Pokemon.id

order by Trainer.name,Pokemon.name;



select Pokemon.name

from Pokemon,Evolution

where Pokemon.id = Evolution.before_id 

	AND Pokemon.id not IN(

		select Evolution.After_id

		from Evolution

	)

	AND Pokemon.id not IN(

		select Before_id

		from Evolution

		where After_id IN (

			select Before_id

			from Evolution

		 )

	)

order by Pokemon.name;



select CatchedPokemon.nickname

from Trainer,Gym,CatchedPokemon,Pokemon

where Trainer.id = Gym.leader_id and Gym.city = 'Sangnok City' and Trainer.id = CatchedPokemon.owner_id and CatchedPokemon.pid = Pokemon.id and Pokemon.type = 'Water'

order by nickname;



select A.name

from (

  select T.name,CP.pid,Count(CP.id) AS cnt

  from Trainer T,CatchedPokemon CP,Evolution E

  where T.id = CP.owner_id and CP.pid = E.After_id

  group by T.name

  ) A

where A.cnt >=3

order by A.name;



select DISTINCT Pokemon.name

from Pokemon,CatchedPokemon

where Pokemon.id not IN (

   select CatchedPokemon.pid

  from CatchedPokemon

  )

order by Pokemon.name;



select maxl 

from( 

	select hometown,T.name,max(CP.level) AS maxl,CP.owner_id 

	from Trainer T,CatchedPokemon CP where T.id = CP.owner_id 

	Group by hometown 

	) A 

order by maxl DESC;



select A.id1,A.n1,A.n2,A.n3

from (

	select P1.name AS n1,P1.id AS id1,P2.id AS id2,P2.name AS n2,P3.name AS n3,P3.id AS id3

	from Pokemon P1,Pokemon P2,Pokemon P3,Evolution E1,Evolution E2

	where P1.id = E1.Before_id and P2.id = E1.AFter_id and P2.id = E2.Before_id and P3.id = E2.After_id

    ) A

order by A.id1;

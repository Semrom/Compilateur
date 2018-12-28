entier $a, entier $b, entier $c;
main()
{
	$a = 1;
	$b = 2;
	$c = 3;

	si $a < $b alors
    {
        ecrire( $b );
    }
    sinon
    {
        ecrire( $a );
    }

	ecrire($c);

	si $a = $b alors
    {
        ecrire( $b );
    }
    sinon
    {
        ecrire( $a );
    }

    ecrire($c);
}
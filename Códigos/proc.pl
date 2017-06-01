#!/usr/bin/env perl

$ejecuciones=30;

for $a(1..$ejecuciones){
  $temp=`./procedural`;
  if($temp =~ /total:\s+(\d+.\d+)/)
  {
    print "Tiempo de ejecución $aº: ",$1, "\n";
    $acumulador += $1;
  }
}

printf "Tiempo promedio tomando $ejecuciones ejecuciones: %.5f\n",$acumulador/$ejecuciones;

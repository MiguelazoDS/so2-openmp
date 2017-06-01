#!/usr/bin/env perl

use Chart::Gnuplot;

my $ejecuciones=100;
my @tiempos;
my @hilos;

for $var(1..10)
{
  for $a(1..$ejecuciones){
     $temp=`./paralelo $var`;
    if($temp =~ /total:\s+(\d+.\d+)/)
    {
      $acumulador += $1;
    }
  }
  printf "Tiempo promedio tomando $ejecuciones ejecuciones con $var hilos: %.5f\n",$acumulador/$ejecuciones;
  push @tiempos, $acumulador/$ejecuciones;
  push @hilos, $var;
  $acumulador=0;
}

my $chart = Chart::Gnuplot->new(
  output => "grafico.jpg",
);

my $dataset = Chart::Gnuplot::DataSet->new(
  xdata => \@hilos,
  ydata => \@tiempos,
  style => "line",
  title => "hilos vs tiempo",
);

$chart -> plot2d($dataset);

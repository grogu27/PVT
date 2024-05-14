#include "../include/include.h"

const float G = 6.67e-11;
omp_lock_t *locks;

double S(double time_nomp,double time_omp){
    return time_nomp/time_omp;
}

int main()
{
    double TtotalSerial = run_serial();
    write_serial(TtotalSerial);
    for(int i = 2; i<5; i++){
        double TtotalOneCriticle = run_one_criticle(i);
        double TtotalSixAtomar = run_six_atomar(i);
        double TtotalNBlock = run_nblock(i);
        double TtotalRedundantCalculations = run_redundant_calculations(i);
        double TtotalStreamStorage = run_stream_storage(i);
        double ResOneCriticle = S(TtotalSerial,TtotalOneCriticle);
        double ResSixAtomar = S(TtotalSerial,TtotalSixAtomar);
        double ResNBlock = S(TtotalSerial,TtotalNBlock);
        double ResRedundantCalculations = S(TtotalSerial,TtotalRedundantCalculations);
        double ResStreamStorage = S(TtotalSerial,TtotalStreamStorage);
        printf("One critical result: %f\n",ResOneCriticle);
        printf("Six atomar result: %f\n",ResSixAtomar);
        printf("N Block result: %f\n",ResNBlock);
        printf("Redundant calculations result: %f\n",ResRedundantCalculations);
        printf("Stream storage result: %f\n",ResStreamStorage);
        write_one_criticle(ResOneCriticle,i);
        write_six_atomar(ResSixAtomar,i);
        write_nblock(ResNBlock,i);
        write_redundant_calculations(ResRedundantCalculations,i);
        write_stream_storage(ResStreamStorage,i);
        
    }
    return 0;
}
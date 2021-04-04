    n_aSetVolume(ptr++, A_RATE, e->em_ltgt, e->em_lratm, e->em_lratl);
    n_aSetVolume(ptr++, A_LEFT  | A_VOL, e->em_cvolL, e->em_dryamt, e->em_wetamt);
    n_aSetVolume(ptr++, A_RIGHT | A_VOL, e->em_rtgt, e->em_rratm,  e->em_rratl);
    n_aEnvMixer (ptr++, A_INIT, e->em_cvolR, osVirtualToPhysical(e->em_state));
}
  else
    n_aEnvMixer(ptr++, A_CONTINUE, 0, osVirtualToPhysical(e->em_state));

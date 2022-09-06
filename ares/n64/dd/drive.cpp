auto DD::seekTrack() -> n1 {
  n16 trackCalc = io.currentTrack.bit(0,11);
  n1 headCalc = io.currentTrack.bit(12);
  u16 trackPhysicalTable[] = {0x09E, 0x13C, 0x1D1, 0x266, 0x2FB, 0x390, 0x425};
  n8 pzone = 0;
  for(u32 n : range(7)) {
    if(trackCalc >= trackPhysicalTable[n]) pzone++;
  }
  pzone += headCalc;

  //return 1 if ROM area, return 0 if RAM area
  if (pzone <= (ctl.diskType + 2)) return 1;
  return 0;
}

auto DD::seekSector(n8 sector) -> u32 {
  n1 blockCalc = (sector >= 0x5A) ? 1 : 0;
  n8 sectorCalc = sector % 0x5A;
  n16 trackCalc = io.currentTrack.bit(0,11);
  n1 headCalc = io.currentTrack.bit(12);

  u32 startOffsetTable[16] = {0x0,0x5F15E0,0xB79D00,0x10801A0,0x1523720,0x1963D80,0x1D414C0,0x20BBCE0,
	                            0x23196E0,0x28A1E00,0x2DF5DC0,0x3299340,0x36D99A0,0x3AB70E0,0x3E31900,0x4149200};
  
  u16 trackPhysicalTable[] = {0x000, 0x09E, 0x13C, 0x1D1, 0x266, 0x2FB, 0x390, 0x425};
  n8 pzone = 0;
  for(u32 n : range(7)) {
    if(trackCalc >= trackPhysicalTable[n + 1]) pzone++;
  }
  trackCalc -= trackPhysicalTable[pzone];
  pzone += (headCalc) ? 8 : 0;

  u32 offsetCalc = startOffsetTable[pzone];
  offsetCalc += (0x55 * (io.sectorSizeBuf + 1)) * 2 * trackCalc;
  offsetCalc += blockCalc * (0x55 * (io.sectorSizeBuf + 1));
  offsetCalc += sectorCalc * (io.sectorSizeBuf + 1);

  //return disk data offset
  return offsetCalc;
}

auto DD::bmRequest() -> void {
  //if BM not started make sure to not do anything
  if(!io.bm.start) {
    queue.remove(Queue::DD_BM_Request);
    lower(IRQ::BM);
    return;
  }

  //reset register state
  io.status.requestUserSector = 0;
  io.status.requestC2Sector = 0;
  io.micro.error = 0;
  io.bm.error = 0;
  io.bm.c1Single = 0;
  io.bm.c1Double = 0;

  n1 blockCalc = (io.currentSector >= 0x5A) ? 1 : 0;
  n8 sectorCalc = io.currentSector - (blockCalc * 0x5A);
  n16 trackCalc = io.currentTrack.bit(0,12);

  if(io.bm.readMode) {
    //read mode
    if(trackCalc == 6) {
      //copy protection (C1 fail all over, retail disk only)
      io.bm.c1Single = 1;
      io.bm.c1Double = 1;
    }

    if(sectorCalc < 0x55) {
      //user sector
      auto offsetCalc = seekSector(io.currentSector);
      for(u32 n : range(io.sectorSizeBuf + 1)) {
        ds.write<Byte>(n, disk.read<Byte>(offsetCalc + n));
      }
      io.status.requestUserSector = 1;
      io.currentSector++;
    } else if (sectorCalc < 0x58) {
      //c2 sector
      io.currentSector++;
    } else if (sectorCalc == 0x58) {
      //last c2 sector
      io.status.requestC2Sector = 1;
      if (io.bm.blockTransfer) {
        //wrap
        io.bm.blockTransfer = 0;
        sectorCalc = 0;
        blockCalc = 1 - blockCalc;
        io.currentSector = sectorCalc + (blockCalc * 0x5A);
      } else {
        //stop
        io.bm.start = 0;
      }
    }
  } else {
    //write mode
    if(sectorCalc <= 0x55) {
      if (sectorCalc > 0) {
        auto offsetCalc = seekSector(io.currentSector-1);
        for(u32 n : range(io.sectorSizeBuf + 1)) {
          disk.write<Byte>(offsetCalc + n, ds.read<Byte>(n));
        }
      }
      io.status.requestUserSector = 1;
    }

    if (sectorCalc >= 0x55) {
      if (io.bm.blockTransfer) {
        io.bm.blockTransfer = 0;
        sectorCalc = 0;
        blockCalc = 1 - blockCalc;
        io.currentSector = sectorCalc + (blockCalc * 0x5A);
      } else {
        if(sectorCalc >= 0x56) {
          io.bm.start = 0;
        }
      }
    }

    io.currentSector++;
  }

  queue.insert(Queue::DD_BM_Request, 50'000);
  raise(IRQ::BM);
}

package com.projetos.g.apimonitoramentoserver.controllers;


import com.projetos.g.apimonitoramentoserver.model.dto.DeviceDTO;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;


import java.util.ArrayList;
import java.util.List;

@RestController
@RequestMapping(value = "/v1/devices")
public class DeviceController {

    @PostMapping(consumes = MediaType.APPLICATION_JSON_VALUE, produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity<DeviceDTO> save(@RequestBody DeviceDTO dto){
        return ResponseEntity.ok(dto);
    }

    @PutMapping(consumes = MediaType.APPLICATION_JSON_VALUE, produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity<DeviceDTO> update(@RequestBody DeviceDTO dto){
        return ResponseEntity.ok(dto);
    }

    @GetMapping(produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity<List<DeviceDTO>> findAll(){
        List<DeviceDTO> list = new ArrayList<>();
        DeviceDTO dto = new DeviceDTO();
        dto.setId(1L);
        dto.setLocal("Piscina Climatizada");
        dto.setSerial("esp8266a");
        dto.setSoftware("temp 1.0");
        dto.setStatus(true);
        list.add(dto);
        return ResponseEntity.ok(list);
    }

    @GetMapping(value = "/{id}", produces = MediaType.APPLICATION_JSON_VALUE)
    public ResponseEntity<DeviceDTO>  findById(@PathVariable Long id){
        List<DeviceDTO> list = new ArrayList<>();
        DeviceDTO device1 = new DeviceDTO();
        device1.setId(1L);
        device1.setLocal("Piscina Climatizada");
        device1.setSerial("esp8266a");
        device1.setSoftware("temp 1.0");
        device1.setStatus(true);

        DeviceDTO device2 = new DeviceDTO();
        device2.setId(2L);
        device2.setLocal("Potaria de entrada");
        device2.setSerial("esp8266b");
        device2.setSoftware("ilum 1.2");
        device2.setStatus(true);
        list.add(device1);
        list.add(device2);

        DeviceDTO dtoSelect = list.stream().filter(x -> 0 == x.getId().compareTo(id)).findFirst().get();
        return ResponseEntity.ok(dtoSelect);
    }
}

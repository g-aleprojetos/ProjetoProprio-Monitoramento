package com.projetos.g.apimonitoramentoserver.model.dto;

import lombok.Getter;
import lombok.Setter;
import org.jetbrains.annotations.NotNull;

@Getter
@Setter
public class DeviceDTO {

    private Long id;
    private String local;
    private String serial;
    private String software;
    private Boolean status;


}

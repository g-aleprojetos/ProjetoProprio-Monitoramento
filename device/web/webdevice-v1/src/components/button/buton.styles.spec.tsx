import * as React from 'react';
import '@testing-library/jest-dom';
import { render} from '@testing-library/react'
import { StyleSheet } from './button.styled';


describe('Botao.styles', ()=>{
    test('DEVE ser igual ao snapshort', ()=>{
        const botao = render(<StyleSheet/>)
        expect(botao).toMatchSnapshot();
    })
});
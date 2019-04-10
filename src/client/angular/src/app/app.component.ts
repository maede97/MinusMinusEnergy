import {Component, Inject, OnInit} from '@angular/core';

import { WEB3 } from './web3';
import Web3 from 'web3';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent implements OnInit{
  title = 'MME Demo Frontend';
  constructor(@Inject(WEB3) private web3: Web3) { }
  async ngOnInit() {
    if ('enable' in this.web3.currentProvider) {
      // @ts-ignore
      await this.web3.currentProvider.enable();
    }
    const accounts = await this.web3.eth.getAccounts();
    console.log(accounts);
  }
}

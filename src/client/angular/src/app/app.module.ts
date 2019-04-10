import { BrowserModule } from '@angular/platform-browser';
import { NgModule, ModuleWithProviders } from '@angular/core';


import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { FlexLayoutModule } from '@angular/flex-layout';
import { LayoutModule } from '@angular/cdk/layout';
import { MDBBootstrapModule } from 'angular-bootstrap-md';
import { MaterialModule} from './material.module';

import { HomeDashboardComponent } from './home-dashboard/home-dashboard.component';
import { BillComponent } from './bill/bill.component';
import { BillDashboardComponent } from './bill-dashboard/bill-dashboard.component';

// https://auth0.com/blog/creating-beautiful-apps-with-angular-material/

@NgModule({
  declarations: [
    AppComponent,
    HomeDashboardComponent,
    BillComponent,
    BillDashboardComponent,
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    BrowserAnimationsModule,
    LayoutModule,
    MaterialModule,
    FlexLayoutModule,
    MDBBootstrapModule,
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }

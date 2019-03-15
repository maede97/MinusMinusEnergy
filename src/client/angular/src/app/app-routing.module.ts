import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { MaterialDashboardComponent} from './material-dashboard/material-dashboard.component';
import { BillComponent} from './bill/bill.component';

const routes: Routes = [
  {path: '', component: MaterialDashboardComponent},
  {path: 'bill', component: BillComponent},
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }

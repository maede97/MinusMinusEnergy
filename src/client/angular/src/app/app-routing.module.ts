import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { HomeDashboardComponent} from './home-dashboard/home-dashboard.component';
import { BillDashboardComponent} from './bill-dashboard/bill-dashboard.component';

const routes: Routes = [
  {path: '', component: HomeDashboardComponent},
  {path: 'bill', component: BillDashboardComponent},
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }

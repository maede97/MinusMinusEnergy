import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { BillDashboardComponent } from './bill-dashboard.component';

describe('BillDashboardComponent', () => {
  let component: BillDashboardComponent;
  let fixture: ComponentFixture<BillDashboardComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ BillDashboardComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(BillDashboardComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});

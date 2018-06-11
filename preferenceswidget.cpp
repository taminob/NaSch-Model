#include "preferenceswidget.h"
#include "graphiccore.h"
#include "core.h"
#include <QKeyEvent>
#include <QPushButton>
#include <QMessageBox>

PreferencesWidget::PreferencesWidget(QWidget* parent) : QFrame(parent), new_game_required(false)
{
	// grey transparent background
	this->setAutoFillBackground(true);
	this->setPalette(QPalette(QColor(0x40, 0x40, 0x40, 0xE9)));

	// dark border around widget
	this->setFrameShadow(QFrame::Raised);
	this->setFrameShape(QFrame::Panel);
	this->setLineWidth(2);

	// setup gui
	init_GUI();
}

void PreferencesWidget::init_GUI()
{
	init_control();
	init_language();
	init_game();
	init_other();

	// add buttons and groups to main layout
	main_layout.addWidget(&hide_button, 0, 0);
	main_layout.addLayout(&language_layout, 0, 1);
	main_layout.addLayout(&game_layout, 1, 0, 4, 1);
	main_layout.addLayout(&appearance_layout, 1, 1, 2, 1);
//	main_layout.addWidget(&application_group, 3, 1, 2, 1);
//	main_layout.addWidget(&color_group, 1, 2, 4, 1);
	main_layout.addWidget(&reset_button, 6, 0);
	main_layout.addWidget(&discard_button, 6, 1);
	main_layout.addWidget(&apply_button, 6, 2);

	// set texts
	translate();
	// set values
	load_values();

	this->setLayout(&main_layout);
}

void PreferencesWidget::init_control()
{
	// set sizepolicy and connect apply button
	apply_button.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
	QObject::connect(&apply_button, &QPushButton::clicked, this, &PreferencesWidget::apply);
	QObject::connect(&apply_button, &QPushButton::clicked, [this]()
	{
		if(new_game_required)
			QMessageBox::information(this, tr("New Game Required"), tr("Some changes require a new game to get applied."), QMessageBox::Ok);

		new_game_required = false;
	});

	// set sizepolicy and connect discard button
	discard_button.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
	QObject::connect(&discard_button, &QPushButton::clicked, this, &PreferencesWidget::discard);

	// set sizepolicy and connect reset button
	reset_button.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	QObject::connect(&reset_button, &QPushButton::clicked, this, &PreferencesWidget::reset);

	// set arrow and sizepolicy and connect hide button (emit hide_preferences signal)
	hide_button.setArrowType(Qt::RightArrow);
	hide_button.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
	QObject::connect(&hide_button, &QToolButton::clicked, [this]() { emit hide_preferences(); });
}

void PreferencesWidget::init_language()
{
	// init english button (emit language_changed signal)
	english_button.setIcon(QIcon(":/images/usa-flag-90.png"));
	english_button.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	QObject::connect(&english_button, &QToolButton::clicked, [this]()
	{
		GraphicCore::get_instance()->get_config()->set_language(Language::English);
		emit language_changed();
	});

	// init german button (emit language_changed signal)
	german_button.setIcon(QIcon(":/images/germany-flag-90.png"));
	german_button.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	QObject::connect(&german_button, &QToolButton::clicked, [this]()
	{
		GraphicCore::get_instance()->get_config()->set_language(Language::German);
		emit language_changed();
	});

	// language buttons to layout
	language_layout.addWidget(&english_button);
	language_layout.addWidget(&german_button);
}

void PreferencesWidget::init_game()
{
	length_input.setMinimum(1);
	length_input.setMaximum(10000);
	QObject::connect(&length_input, &QSpinBox::editingFinished, [this]() { Core::get_instance()->get_config()->set_street_length(length_input.value()); });
	lanes_input.setMinimum(1);
	lanes_input.setMaximum(10000);
	QObject::connect(&lanes_input, &QSpinBox::editingFinished, [this]() { Core::get_instance()->get_config()->set_street_lanes(lanes_input.value()); });

	car_density_input.setMinimum(0);
	car_density_input.setMaximum(100);
	QObject::connect(&car_density_input, &QSpinBox::editingFinished, [this]() { Core::get_instance()->get_config()->set_car_density(car_density_input.value()); });

	slow_down_chance_input.setMinimum(0);
	slow_down_chance_input.setMaximum(100);
	QObject::connect(&slow_down_chance_input, &QSpinBox::editingFinished, [this]() { Core::get_instance()->get_config()->set_slow_down_chance(slow_down_chance_input.value()); });

	max_speed_input.setMinimum(1);
	max_speed_input.setMaximum(1000);
	QObject::connect(&max_speed_input, &QSpinBox::editingFinished, [this]() { Core::get_instance()->get_config()->set_max_speed(max_speed_input.value()); });

	game_layout.addWidget(&length_text, 0, 0);
	game_layout.addWidget(&length_input, 0, 1);
	game_layout.addWidget(&lanes_text, 1, 0);
	game_layout.addWidget(&lanes_input, 1, 1);
	game_layout.addWidget(&car_density_text, 2, 0);
	game_layout.addWidget(&car_density_input, 2, 1);
	game_layout.addWidget(&car_density_unit, 2, 2);
	game_layout.addWidget(&slow_down_chance_text, 3, 0);
	game_layout.addWidget(&slow_down_chance_input, 3, 1);
	game_layout.addWidget(&slow_down_chance_unit, 3, 2);
	game_layout.addWidget(&max_speed_text, 4, 0);
	game_layout.addWidget(&max_speed_input, 4, 1);
}

void PreferencesWidget::init_other()
{
	QObject::connect(&break_long_streets_input, &QCheckBox::clicked, [this]()
	{
		GraphicCore::get_instance()->get_config()->set_long_street_break(break_long_streets_input.isChecked());
		GraphicCore::get_instance()->update();
	});

	appearance_layout.addWidget(&break_long_streets_input, 0, 0);
}

void PreferencesWidget::translate()
{
	length_text.setText(tr("Street Length"));
	lanes_text.setText(tr("Street Lanes"));
	car_density_text.setText(tr("Car Density"));
	car_density_unit.setText("%");
	slow_down_chance_text.setText(tr("Propability Of Deceleration"));
	slow_down_chance_unit.setText("%");
	max_speed_text.setText(tr("Maximum Speed"));

	break_long_streets_input.setText(tr("Break Long Streets"));
}

void PreferencesWidget::load_values()
{
	length_input.setValue(Core::get_instance()->get_config()->get_street_length());
	lanes_input.setValue(Core::get_instance()->get_config()->get_street_lanes());
	car_density_input.setValue(Core::get_instance()->get_config()->get_car_density());
	slow_down_chance_input.setValue(Core::get_instance()->get_config()->get_slow_down_chance());
	max_speed_input.setValue(Core::get_instance()->get_config()->get_max_speed());

	break_long_streets_input.setChecked(GraphicCore::get_instance()->get_config()->get_long_street_break());
}

void PreferencesWidget::apply()
{
	// if writing config fails
	if(!(Core::get_instance()->get_config()->write_config() && GraphicCore::get_instance()->get_config()->write_config()))
		QMessageBox::warning(this, tr("Write Error"), tr("Writing Configuration Failed!\nPlease Check Your Permissions."));

	GraphicCore::get_instance()->update();
}

void PreferencesWidget::discard()
{
	// read in saved preferences again
	Core::get_instance()->get_config()->read_config();
	GraphicCore::get_instance()->get_config()->read_config();

	// update GUI
	load_values();
	GraphicCore::get_instance()->update();
}

void PreferencesWidget::reset()
{
	// verify reset by user
	if(QMessageBox::warning(this, tr("Reset Preferences?"), tr("These Changes Can Not Be Undone!\nReset Preferences To Default?"), QMessageBox::Yes | QMessageBox::No)
			!= QMessageBox::Yes)
		return;

	// set config to default
	Core::get_instance()->get_config()->reset_config();
	GraphicCore::get_instance()->get_config()->reset_config();
	// update values in preferences view
	load_values();

//	emit color_changed();
	emit language_changed();

	// apply reset
	apply();
	QMessageBox::information(this, tr("New Game Required"), tr("Some changes may require a new game to get applied."), QMessageBox::Ok);
}

void PreferencesWidget::enable_focus()
{
	// allow focus to all children
	for(QObject* a : this->children())
	{
		auto b = dynamic_cast<QWidget*>(a);
		if(b)
			b->setFocusPolicy(Qt::StrongFocus);
	}

	// enable own focus
	this->setFocusPolicy(Qt::StrongFocus);
}

void PreferencesWidget::disable_focus()
{
	// revoke focus of all children
	for(QObject* a : this->children())
	{
		auto b = dynamic_cast<QWidget*>(a);
		if(b)
			b->setFocusPolicy(Qt::NoFocus);
	}

	// disable own focus
	this->setFocusPolicy(Qt::NoFocus);
}

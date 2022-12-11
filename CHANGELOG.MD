# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/en/1.0.0/)
and this project adheres to [Semantic Versioning](http://semver.org/spec/v2.0.0.html).

## [v2.1.0](https://github.com/sillydan1/expr/releases/tag/v2.1.0) - 2022-12-11

<small>[Compare with v2.0.0](https://github.com/sillydan1/expr/compare/v2.0.0...v2.1.0)</small>

### Bug Fixes
- Add a lookup function to symbol table ([797cdee](https://github.com/sillydan1/expr/commit/797cdeeb170cc1aa465adc0331efafe8a616b671) by Asger Gitz-Johansen).
- Removed tree_interpreter functionality in the demo ([1563f5f](https://github.com/sillydan1/expr/commit/1563f5f0689f45371d14bf4752d9998e23d681f4) by Asger Gitz-Johansen).

### Features
- Add as_string function ([b1bf12a](https://github.com/sillydan1/expr/commit/b1bf12a71cfc4270572b430274e979773ae945a2) by Asger Gitz-Johansen).
- Add tree_compiler class ([feba171](https://github.com/sillydan1/expr/commit/feba17187e915172d2dbb832a089291839611475) by Asger Gitz-Johansen).
- Extend the driver interface be aware of the access modifier ([418ce71](https://github.com/sillydan1/expr/commit/418ce71200423b321767698743ae754ed5cb1e78) by Asger Gitz-Johansen).
- Add variables scopes in the form of trees of symboltables ([8f5ec2b](https://github.com/sillydan1/expr/commit/8f5ec2b9de720562d1ad931ba29dc2f0283699de) by Asger Gitz-Johansen).


## [v2.0.0](https://github.com/sillydan1/expr/releases/tag/v2.0.0) - 2022-09-19

<small>[Compare with v1.7.0](https://github.com/sillydan1/expr/compare/v1.7.0...v2.0.0)</small>

### Bug Fixes
- Warning about not returning at the end of a non-void function ([4e6ea47](https://github.com/sillydan1/expr/commit/4e6ea4726767f3e4895a49bb9e10782d62552922) by Asger Gitz-Johansen).
- Mingw pipeline ([de346ee](https://github.com/sillydan1/expr/commit/de346eee8bd1c0acc6f8b74ad5509a0b126c567a) by Asger Gitz-Johansen).
- Fixing gcc-related compilation issues ([859f8f3](https://github.com/sillydan1/expr/commit/859f8f34dc9395dc56bdd60c58845344237876e6) by Asger Gitz-Johansen).
- Symbol_table_t now contains delay information ([6ef9e08](https://github.com/sillydan1/expr/commit/6ef9e0834a417f22e21afb6de4a71cfe169dbd4b) by Asger Gitz-Johansen).
- Z3 bug where known variables were interpreted as "known" ([f3430b0](https://github.com/sillydan1/expr/commit/f3430b02d4030abf171a9fa98849ceda00fdaf84) by Asger Gitz-Johansen).
- Move driver.h into include directory ([43ecd13](https://github.com/sillydan1/expr/commit/43ecd13525c835ff66b79a443aa92f6b6cb920ec) by Asger Gitz-Johansen).
- Fixed identifier bug in the z3_driver ([f2e6fbd](https://github.com/sillydan1/expr/commit/f2e6fbd6360f57afee0010abe7757a531819493e) by Asger Gitz-Johansen).

### Code Refactoring
- Identifiers are now stored as a string instead of a iterator ([c6dae37](https://github.com/sillydan1/expr/commit/c6dae37fd241ac53401dd7cd0087c866f881f0cd) by Asger Gitz-Johansen).

### Features
- Add special case in z3_driver for clocks ([92806fa](https://github.com/sillydan1/expr/commit/92806fade71e1b5de012c331a5a2219e40442d45) by Asger Gitz-Johansen).
- Add c++ comparator operators for symbol_value_t ([29cbe74](https://github.com/sillydan1/expr/commit/29cbe74d2a36401c207ec3a495221b7b445e0e3b) by Asger Gitz-Johansen).
- Add support for delayable clock types ([a6aae17](https://github.com/sillydan1/expr/commit/a6aae172e3e1dac98b68ea6896be67c5a1feaab3) by Asger Gitz-Johansen).


## [v1.7.0](https://github.com/sillydan1/expr/releases/tag/v1.7.0) - 2022-09-11

<small>[Compare with v1.6.0](https://github.com/sillydan1/expr/compare/v1.6.0...v1.7.0)</small>

### Bug Fixes
- Cleanup interpreter::evaluate function ([137fdad](https://github.com/sillydan1/expr/commit/137fdad5284563f22ff52841b8ce5b9e90a5848e) by Asger Gitz-Johansen).
- Interpreter now does an environment lookup for all identifier references ([e3250f5](https://github.com/sillydan1/expr/commit/e3250f53716d0d3ebc24e858dadd3ba72a36dfed) by Asger Gitz-Johansen).
- Add space between the macro and parentheses ([0a7b2a2](https://github.com/sillydan1/expr/commit/0a7b2a213d8bf2ceca25531d90707f274f87b9cf) by Asger Gitz-Johansen).
- Add m4_define_default parser_ns for namespace overwritability ([8b896b1](https://github.com/sillydan1/expr/commit/8b896b1334840adb3c8152720c072dee4a6d4167) by Asger Gitz-Johansen).

### Features
- Add interpret_declarations and interpret_expression functions to interpreter ([2ccc3e6](https://github.com/sillydan1/expr/commit/2ccc3e69f6271009d0d28b54ea5beb1251d96e4a) by Asger Gitz-Johansen).


## [v1.6.0](https://github.com/sillydan1/expr/releases/tag/v1.6.0) - 2022-08-23

<small>[Compare with v1.5.0](https://github.com/sillydan1/expr/compare/v1.5.0...v1.6.0)</small>


## [v1.5.0](https://github.com/sillydan1/expr/releases/tag/v1.5.0) - 2022-07-19

<small>[Compare with v1.4.1](https://github.com/sillydan1/expr/compare/v1.4.1...v1.5.0)</small>


## [v1.4.1](https://github.com/sillydan1/expr/releases/tag/v1.4.1) - 2022-07-19

<small>[Compare with v1.4.0](https://github.com/sillydan1/expr/compare/v1.4.0...v1.4.1)</small>


## [v1.4.0](https://github.com/sillydan1/expr/releases/tag/v1.4.0) - 2022-07-19

<small>[Compare with v1.3.2](https://github.com/sillydan1/expr/compare/v1.3.2...v1.4.0)</small>


## [v1.3.2](https://github.com/sillydan1/expr/releases/tag/v1.3.2) - 2022-05-02

<small>[Compare with v1.3.0](https://github.com/sillydan1/expr/compare/v1.3.0...v1.3.2)</small>


## [v1.3.0](https://github.com/sillydan1/expr/releases/tag/v1.3.0) - 2022-04-07

<small>[Compare with v1.2.0](https://github.com/sillydan1/expr/compare/v1.2.0...v1.3.0)</small>


## [v1.2.0](https://github.com/sillydan1/expr/releases/tag/v1.2.0) - 2022-03-20

<small>[Compare with v1.1.3](https://github.com/sillydan1/expr/compare/v1.1.3...v1.2.0)</small>


## [v1.1.3](https://github.com/sillydan1/expr/releases/tag/v1.1.3) - 2022-03-06

<small>[Compare with v1.1.2](https://github.com/sillydan1/expr/compare/v1.1.2...v1.1.3)</small>


## [v1.1.2](https://github.com/sillydan1/expr/releases/tag/v1.1.2) - 2022-03-05

<small>[Compare with v1.1.0](https://github.com/sillydan1/expr/compare/v1.1.0...v1.1.2)</small>


## [v1.1.0](https://github.com/sillydan1/expr/releases/tag/v1.1.0) - 2022-03-05

<small>[Compare with v1.0.0](https://github.com/sillydan1/expr/compare/v1.0.0...v1.1.0)</small>


## [v1.0.0](https://github.com/sillydan1/expr/releases/tag/v1.0.0) - 2022-03-05

<small>[Compare with first commit](https://github.com/sillydan1/expr/compare/8337824c2e8488a3226b773b345b0d5b537c3a7a...v1.0.0)</small>


